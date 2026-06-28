#include "MCTSAgent.h"
#include <cmath>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iostream>

MCTSNode::MCTSNode(MCTSNode* parent, Action a1, Action a2) 
    : visits(0), winScore(0.0f), p1Action(a1), p2Action(a2), parent(parent) {}

bool MCTSNode::isFullyExpanded(int numP1Actions, int numP2Actions) const {
    return children.size() == static_cast<size_t>(numP1Actions * numP2Actions);
}

MCTSAgent::MCTSAgent(int iters) : iterations(iters) {}

std::vector<Action> MCTSAgent::generateLegalActions(const GameState& state, int playerId) {
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
    
    for (int i = 1; i < trainer.party.size(); ++i) {
        if (trainer.party[i].isAlive()) {
            actions.push_back({ActionType::SWITCH, i});
        }
    }
    
    if (trainer.potions > 0 && active->current_hp < active->max_hp) {
        actions.push_back({ActionType::POTION, 0});
    }
    
    return actions;
}

GameState MCTSAgent::determinize(const GameState& state, int playerId) {
    // Information Set logic: Normally we would randomize hidden elements of the enemy.
    // For this prototype, all moves are known, but we could randomly alter the enemy's stat stages
    // or unknown items if they were hidden. Here we just return the true state as the "determinized" state.
    return state;
}

int MCTSAgent::simulateRandomPlayout(GameState state, int povPlayerId) {
    int turns = 0;
    while (state.player1.hasAlivePokemon() && state.player2.hasAlivePokemon() && turns < 20) {
        std::vector<Action> p1Actions = generateLegalActions(state, 1);
        std::vector<Action> p2Actions = generateLegalActions(state, 2);
        
        if (p1Actions.empty() || p2Actions.empty()) break;
        
        Action a1 = p1Actions[std::rand() % p1Actions.size()];
        Action a2 = p2Actions[std::rand() % p2Actions.size()];
        
        state.step(a1, a2);
        turns++;
    }
    
    float score = state.evaluate(povPlayerId);
    if (score > 0) return 1;
    if (score < 0) return 0;
    return 0; // tie/loss
}

Action MCTSAgent::getAction(const GameState& state, int playerId) {
    int enemyPlayerId = (playerId == 1) ? 2 : 1;
    std::vector<Action> myActions = generateLegalActions(state, playerId);
    
    if (myActions.empty()) return {ActionType::MOVE, 0};
    if (myActions.size() == 1) return myActions[0];

    std::vector<float> actionScores(myActions.size(), 0.0f);
    std::vector<int> actionVisits(myActions.size(), 0);

    // MCTS Loop
    for (int i = 0; i < iterations; ++i) {
        // 1. Determinization
        GameState simState = determinize(state, playerId);
        
        // 2. We use a flat Monte Carlo approach for simultaneous moves at the root
        // Decoupled UCT:
        int bestActionIdx = -1;
        float bestUCB = -std::numeric_limits<float>::max();
        
        for (int a = 0; a < myActions.size(); ++a) {
            float ucb;
            if (actionVisits[a] == 0) {
                ucb = 99999.0f; // explore unvisited
            } else {
                float exploitation = actionScores[a] / actionVisits[a];
                float exploration = 1.41f * std::sqrt(std::log((float)i + 1.0f) / actionVisits[a]);
                ucb = exploitation + exploration;
            }
            if (ucb > bestUCB) {
                bestUCB = ucb;
                bestActionIdx = a;
            }
        }
        
        // Assume random enemy action for the rollout
        std::vector<Action> enemyActions = generateLegalActions(simState, enemyPlayerId);
        Action enemyAction = enemyActions[std::rand() % enemyActions.size()];
        
        Action myAction = myActions[bestActionIdx];
        
        if (playerId == 1) {
            simState.step(myAction, enemyAction);
        } else {
            simState.step(enemyAction, myAction);
        }
        
        // 3. Simulation
        int win = simulateRandomPlayout(simState, playerId);
        
        // 4. Backpropagation
        actionVisits[bestActionIdx]++;
        actionScores[bestActionIdx] += win;
    }

    // Return the action with the most visits (most robust)
    int bestFinalIdx = 0;
    int maxVisits = -1;
    for (int a = 0; a < myActions.size(); ++a) {
        if (actionVisits[a] > maxVisits) {
            maxVisits = actionVisits[a];
            bestFinalIdx = a;
        }
    }

    return myActions[bestFinalIdx];
}
