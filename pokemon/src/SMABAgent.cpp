#include "SMABAgent.h"
#include <algorithm>
#include <limits>

SMABAgent::SMABAgent(int depth) : searchDepth(depth) {}

std::vector<Action> SMABAgent::generateLegalActions(const GameState& state, int playerId) {
    std::vector<Action> actions;
    const Trainer& trainer = (playerId == 1) ? state.player1 : state.player2;
    const Pokemon* active = trainer.getActivePokemon();
    
    // Forced switch scenario
    if (!active) {
        for (int i = 0; i < trainer.party.size(); ++i) {
            if (trainer.party[i].isAlive()) {
                actions.push_back({ActionType::SWITCH, i});
            }
        }
        return actions;
    }
    
    // Moves
    for (int i = 0; i < active->moves.size(); ++i) {
        actions.push_back({ActionType::MOVE, i});
    }
    
    // Switches
    // Start from 1, because 0 is the currently active pokemon
    for (int i = 1; i < trainer.party.size(); ++i) {
        if (trainer.party[i].isAlive()) {
            actions.push_back({ActionType::SWITCH, i});
        }
    }
    
    // Potions
    if (trainer.potions > 0 && active->current_hp < active->max_hp) {
        actions.push_back({ActionType::POTION, 0});
    }
    
    return actions;
}

float SMABAgent::smabSearch(const GameState& state, int depth, int povPlayerId) {
    if (!state.player1.hasAlivePokemon() || !state.player2.hasAlivePokemon() || depth == 0) {
        return state.evaluate(povPlayerId);
    }
    
    int enemyPlayerId = (povPlayerId == 1) ? 2 : 1;
    
    std::vector<Action> myActions = generateLegalActions(state, povPlayerId);
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId);
    
    if (myActions.empty()) return -99999.0f;
    if (enemyActions.empty()) return 99999.0f;
    
    // Maximin logic with Alpha-Beta style dominance pruning
    float bestWorstCase = -std::numeric_limits<float>::max();
    
    for (const auto& myAction : myActions) {
        float worstCaseForThisAction = std::numeric_limits<float>::max();
        
        for (const auto& enemyAction : enemyActions) {
            GameState nextState = state;
            if (povPlayerId == 1) {
                nextState.step(myAction, enemyAction);
            } else {
                nextState.step(enemyAction, myAction);
            }
            
            float score = smabSearch(nextState, depth - 1, povPlayerId);
            if (score < worstCaseForThisAction) {
                worstCaseForThisAction = score;
            }
            
            // Pruning: if the worst case of THIS action is already worse than 
            // the best worst-case we've found in a PREVIOUS action, stop searching.
            if (worstCaseForThisAction <= bestWorstCase) {
                break;
            }
        }
        
        if (worstCaseForThisAction > bestWorstCase) {
            bestWorstCase = worstCaseForThisAction;
        }
    }
    
    return bestWorstCase;
}

Action SMABAgent::getAction(const GameState& state, int playerId) {
    int enemyPlayerId = (playerId == 1) ? 2 : 1;
    
    std::vector<Action> myActions = generateLegalActions(state, playerId);
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId);
    
    if (myActions.empty()) return {ActionType::MOVE, 0}; // Fallback
    if (myActions.size() == 1) return myActions[0]; // Forced action
    
    int bestActionIndex = 0;
    float bestWorstCase = -std::numeric_limits<float>::max();
    
    for (int i = 0; i < myActions.size(); ++i) {
        float worstCaseForThisAction = std::numeric_limits<float>::max();
        
        for (int j = 0; j < enemyActions.size(); ++j) {
            GameState nextState = state;
            if (playerId == 1) {
                nextState.step(myActions[i], enemyActions[j]);
            } else {
                nextState.step(enemyActions[j], myActions[i]);
            }
            
            float score = smabSearch(nextState, searchDepth - 1, playerId);
            if (score < worstCaseForThisAction) {
                worstCaseForThisAction = score;
            }
            
            if (worstCaseForThisAction <= bestWorstCase) {
                break; // Alpha-beta dominance prune
            }
        }
        
        if (worstCaseForThisAction > bestWorstCase) {
            bestWorstCase = worstCaseForThisAction;
            bestActionIndex = i;
        }
    }
    
    return myActions[bestActionIndex];
}
