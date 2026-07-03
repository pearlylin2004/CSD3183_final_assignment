#include "MCTSAgent.h"
#include <cmath>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iostream>
#include "GreedyAgent.h"

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
    // For this prototype, all moves are known
    return state;
}

float MCTSAgent::simulateRandomPlayout(GameState state, int povPlayerId) {
    int turns = 0;
    GreedyAgent greedy;
    
    float initialScore = state.evaluate(povPlayerId);
    
    while (state.player1.hasAlivePokemon() && state.player2.hasAlivePokemon() && turns < 50) {
        Action a1 = greedy.getAction(state, 1);
        Action a2 = greedy.getAction(state, 2);
        
        state.step(a1, a2);
        turns++;
    }
    
    float finalScore = state.evaluate(povPlayerId);
    float deltaScore = finalScore - initialScore;
    
    if (!state.player1.hasAlivePokemon() && povPlayerId == 2) return 1.0f;
    if (!state.player2.hasAlivePokemon() && povPlayerId == 1) return 1.0f;
    if (!state.player1.hasAlivePokemon() || !state.player2.hasAlivePokemon()) return 0.0f;
    
    // Otherwise, convert the HP delta to a win probability (0.0 to 1.0) using a sigmoid
    float winProb = 1.0f / (1.0f + std::exp(-deltaScore / 20.0f));
    return winProb;
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
        GameState simState = determinize(state, playerId);
        
        // flat Monte Carlo approach for simultaneous moves at the root
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
        
        // Use GreedyAgent to predict the opponents action instead of assuming they play randomly
        GreedyAgent greedyRoot;
        Action enemyAction = greedyRoot.getAction(simState, enemyPlayerId);
        
        Action myAction = myActions[bestActionIdx];
        
        if (playerId == 1) {
            simState.step(myAction, enemyAction);
        } else {
            simState.step(enemyAction, myAction);
        }
        
        float win = simulateRandomPlayout(simState, playerId);
        
        actionVisits[bestActionIdx]++;
        actionScores[bestActionIdx] += win;
    }

    // Return the action with the most visits
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
