#include "SMABAgent.h"
#include <algorithm>
#include <limits>

SMABAgent::SMABAgent(int depth) : searchDepth(depth) {}

std::vector<Action> SMABAgent::generateLegalActions(const GameState& state, int playerId, bool assumeNoSwitch) {
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
    if (!assumeNoSwitch) {
        // Start from 1, because 0 is the currently active pokemon
        for (int i = 1; i < trainer.party.size(); ++i) {
            if (trainer.party[i].isAlive()) {
                actions.push_back({ActionType::SWITCH, i});
            }
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
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId, true); // Assume enemy doesn't switch unless forced
    
    if (myActions.empty()) return -99999.0f;
    if (enemyActions.empty()) return 99999.0f;
    
    // Maximin logic with Alpha-Beta style dominance pruning
    float bestWorstCase = -std::numeric_limits<float>::max();
    
    for (const auto& myAction : myActions) {
        float worstCaseForThisAction = std::numeric_limits<float>::max();
        
        for (const auto& enemyAction : enemyActions) {
            std::vector<std::pair<GameState, float>> outcomes;
            if (povPlayerId == 1) {
                outcomes = state.stepExpecti(myAction, enemyAction);
            } else {
                outcomes = state.stepExpecti(enemyAction, myAction);
            }
            
            float expectedScore = 0.0f;
            for (const auto& outcome : outcomes) {
                expectedScore += outcome.second * smabSearch(outcome.first, depth - 1, povPlayerId);
            }
            if (myAction.type == ActionType::SWITCH) {
                expectedScore -= 2.0f; // Small penalty to prevent infinite switching
            }
            
            if (expectedScore < worstCaseForThisAction) {
                worstCaseForThisAction = expectedScore;
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
    std::vector<Action> enemyActions = generateLegalActions(state, enemyPlayerId, true); // Assume enemy doesn't switch unless forced
    
    if (myActions.empty()) return {ActionType::MOVE, 0}; // Fallback
    if (myActions.size() == 1) return myActions[0]; // Forced action
    
    int bestActionIndex = 0;
    float bestWorstCase = -std::numeric_limits<float>::max();
    
    for (int i = 0; i < myActions.size(); ++i) {
        float worstCaseForThisAction = std::numeric_limits<float>::max();
        
        for (int j = 0; j < enemyActions.size(); ++j) {
            std::vector<std::pair<GameState, float>> outcomes;
            if (playerId == 1) {
                outcomes = state.stepExpecti(myActions[i], enemyActions[j]);
            } else {
                outcomes = state.stepExpecti(enemyActions[j], myActions[i]);
            }
            
            float expectedScore = 0.0f;
            for (const auto& outcome : outcomes) {
                expectedScore += outcome.second * smabSearch(outcome.first, searchDepth - 1, playerId);
            }
            if (myActions[i].type == ActionType::SWITCH) {
                expectedScore -= 2.0f; // Small penalty to prevent infinite switching
            }
            
            if (expectedScore < worstCaseForThisAction) {
                worstCaseForThisAction = expectedScore;
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
