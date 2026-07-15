#include "SMABAgent.h"
#include <algorithm>
#include <limits>
#include <iostream>
#include <glpk.h>
#include <utility>
#include <cstdlib>

SMABAgent::SMABAgent(int depth) : searchDepth(depth) {}

std::vector<Action> SMABAgent::generateLegalActions(const GameState& state, int playerId, bool assumeNoSwitch) {
    std::vector<Action> actions;
    const Trainer& trainer = (playerId == 1) ? state.player1 : state.player2;
    const Pokemon* active = trainer.getActivePokemon();
    
    if (!active) {
        for (int i = 0; i < trainer.party.size(); ++i) {
            if (trainer.party[i].isAlive()) {
                actions.push_back({ActionType::SWITCH, i});
            }
        }
        return actions;
    }
    for (int i = 0; i < active->moves.size(); ++i) {
        actions.push_back({ActionType::MOVE, i});
    }
    if (!assumeNoSwitch) {
        for (int i = 1; i < trainer.party.size(); ++i) {
            if (trainer.party[i].isAlive()) {
                actions.push_back({ActionType::SWITCH, i});
            }
        }
    }
    if (trainer.potions > 0 && active->current_hp < active->max_hp) {
        actions.push_back({ActionType::POTION, 0});
    }
    return actions;
}

// no win win or lose lose
std::pair<float, std::vector<float>> solveZeroSumGame(const std::vector<std::vector<float>>& matrix) {
    int numRows = matrix.size();
    if (numRows == 0) return {0.0f, {}};
    int numCols = matrix[0].size();
    if (numCols == 0) return {0.0f, {}};

    glp_prob *lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX);

    glp_add_cols(lp, numRows + 1);
    glp_set_col_name(lp, 1, "v");
    glp_set_col_bnds(lp, 1, GLP_FR, 0.0, 0.0);
    glp_set_obj_coef(lp, 1, 1.0); // Maximize

    for (int i = 1; i <= numRows; ++i) {
        glp_set_col_bnds(lp, i + 1, GLP_DB, 0.0, 1.0);
        glp_set_obj_coef(lp, i + 1, 0.0);
    }

    glp_add_rows(lp, numCols + 1);
    std::vector<int> ia(1 + (numRows + 1) * (numCols + 1));
    std::vector<int> ja(1 + (numRows + 1) * (numCols + 1));
    std::vector<double> ar(1 + (numRows + 1) * (numCols + 1));
    
    int idx = 1;
    for (int j = 1; j <= numCols; ++j) {
        glp_set_row_bnds(lp, j, GLP_UP, 0.0, 0.0);
        ia[idx] = j; ja[idx] = 1; ar[idx] = 1.0; idx++; // v
        for (int i = 1; i <= numRows; ++i) {
            ia[idx] = j; ja[idx] = i + 1; ar[idx] = -matrix[i-1][j-1]; idx++;
        }
    }
    
    glp_set_row_bnds(lp, numCols + 1, GLP_FX, 1.0, 1.0);
    for (int i = 1; i <= numRows; ++i) {
        ia[idx] = numCols + 1; ja[idx] = i + 1; ar[idx] = 1.0; idx++;
    }

    glp_load_matrix(lp, idx - 1, ia.data(), ja.data(), ar.data());
    
    glp_smcp parm;
    glp_init_smcp(&parm);
    parm.msg_lev = GLP_MSG_OFF; // Turn off output
    
    glp_simplex(lp, &parm);
    float value = (float)glp_get_obj_val(lp);
    
    std::vector<float> strategy(numRows, 0.0f);
    for (int i = 1; i <= numRows; ++i) {
        strategy[i-1] = (float)glp_get_col_prim(lp, i + 1);
    }
    
    glp_delete_prob(lp);
    
    return {value, strategy};
}

float SMABAgent::smabSearch(const GameState& state, int depth, int povPlayerId, float alpha, float beta) {
    if (!state.player1.hasAlivePokemon() || !state.player2.hasAlivePokemon() || depth == 0) {
        return state.evaluate(povPlayerId);
    }
    
    int enemyPlayerId = (povPlayerId == 1) ? 2 : 1;
    
    std::vector<Action> myActions = generateLegalActions(state, povPlayerId);
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId, true);
    
    if (myActions.empty()) return -99999.0f;
    if (enemyActions.empty()) return 99999.0f;
    
    int m = myActions.size();
    int n = enemyActions.size();
    
    // Optimistic and Pessimistic matrices
    std::vector<std::vector<float>> P(m, std::vector<float>(n, -99999.0f));
    std::vector<std::vector<float>> O(m, std::vector<float>(n, 99999.0f));
    std::vector<bool> rowDominated(m, false);
    std::vector<bool> colDominated(n, false);

    for (int i = 0; i < m; ++i) {
        if (rowDominated[i]) continue;
        for (int j = 0; j < n; ++j) {
            if (colDominated[j]) continue;
            
            std::vector<std::pair<GameState, float>> outcomes;
            if (povPlayerId == 1) {
                outcomes = state.stepExpecti(myActions[i], enemyActions[j]);
            } else {
                outcomes = state.stepExpecti(enemyActions[j], myActions[i]);
            }
            
            float expectedScore = 0.0f;
            for (const auto& outcome : outcomes) {
                expectedScore += outcome.second * smabSearch(outcome.first, depth - 1, povPlayerId, -99999.0f, 99999.0f);
            }
            
            if (myActions[i].type == ActionType::SWITCH) expectedScore -= 2.0f;
            
            P[i][j] = O[i][j] = expectedScore;
            
            bool isDominated = false;
            for (int k = 0; k < m; ++k) {
                if (k != i && !rowDominated[k]) {
                    bool strictlyBetter = true;
                    for (int c = 0; c <= j; ++c) {
                        if (P[k][c] < O[i][c]) strictlyBetter = false;
                    }
                    if (strictlyBetter) { isDominated = true; break; }
                }
            }
            if (isDominated) {
                rowDominated[i] = true;
                break;
            }
        }
    }
    
    std::vector<std::vector<float>> finalMatrix;
    for (int i = 0; i < m; ++i) {
        if (!rowDominated[i]) {
            std::vector<float> row;
            for (int j = 0; j < n; ++j) {
                if (!colDominated[j]) {
                    row.push_back(P[i][j]);
                }
            }
            finalMatrix.push_back(row);
        }
    }
    
    return solveZeroSumGame(finalMatrix).first;
}

Action SMABAgent::getAction(const GameState& state, int playerId) {
    int enemyPlayerId = (playerId == 1) ? 2 : 1;
    
    std::vector<Action> myActions = generateLegalActions(state, playerId);
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId, true);
    
    if (myActions.empty()) return {ActionType::MOVE, 0};
    if (myActions.size() == 1) return myActions[0];
    
    int m = myActions.size();
    int n = enemyActions.size();
    std::vector<std::vector<float>> matrix(m, std::vector<float>(n, 0.0f));
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::vector<std::pair<GameState, float>> outcomes;
            if (playerId == 1) {
                outcomes = state.stepExpecti(myActions[i], enemyActions[j]);
            } else {
                outcomes = state.stepExpecti(enemyActions[j], myActions[i]);
            }
            
            float expectedScore = 0.0f;
            for (const auto& outcome : outcomes) {
                expectedScore += outcome.second * smabSearch(outcome.first, searchDepth - 1, playerId, -99999.0f, 99999.0f);
            }
            if (myActions[i].type == ActionType::SWITCH) expectedScore -= 2.0f;
            
            matrix[i][j] = expectedScore;
        }
    }
    
    auto result = solveZeroSumGame(matrix);
    std::vector<float> strategy = result.second;
    
    float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    float cumulative = 0.0f;
    for (int i = 0; i < m; ++i) {
        cumulative += strategy[i];
        if (r <= cumulative) {
            return myActions[i];
        }
    }
    
    return myActions[0];
}
