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
    return state;
}

float MCTSAgent::simulateRandomPlayout(GameState state, int povPlayerId) {
    int turns = 0;
    GreedyAgent greedy;
    
    while (state.player1.hasAlivePokemon() && state.player2.hasAlivePokemon() && turns < 30) {
        auto a1s = generateLegalActions(state, 1);
        auto a2s = generateLegalActions(state, 2);
        if (a1s.empty() || a2s.empty()) break;
        
        Action a1 = greedy.getAction(state, 1);
        Action a2 = greedy.getAction(state, 2);
        
        state.step(a1, a2);
        turns++;
    }
    
    if (!state.player1.hasAlivePokemon() && povPlayerId == 2) return 1.0f;
    if (!state.player2.hasAlivePokemon() && povPlayerId == 1) return 1.0f;
    if (!state.player1.hasAlivePokemon() || !state.player2.hasAlivePokemon()) return 0.0f;
    
    float score1 = state.evaluate(1);
    float score2 = state.evaluate(2);
    
    if (povPlayerId == 1) {
        return score1 > score2 ? 1.0f : 0.0f;
    } else {
        return score2 > score1 ? 1.0f : 0.0f;
    }
}

Action MCTSAgent::getAction(const GameState& state, int playerId) {
    int enemyPlayerId = (playerId == 1) ? 2 : 1;
    auto myActions = generateLegalActions(state, playerId);
    
    if (myActions.empty()) return {ActionType::MOVE, 0};
    if (myActions.size() == 1) return myActions[0];

    auto root = std::make_unique<MCTSNode>();

    for (int i = 0; i < iterations; ++i) {
        MCTSNode* node = root.get();
        GameState simState = determinize(state, playerId);

        while (node->isFullyExpanded(generateLegalActions(simState, 1).size(), generateLegalActions(simState, 2).size()) && !node->children.empty()) {
            MCTSNode* bestChild = nullptr;
            float bestUCB1 = -1e9f, bestUCB2 = -1e9f;
            Action bestA1 = {ActionType::MOVE, 0}, bestA2 = {ActionType::MOVE, 0};

            auto a1s = generateLegalActions(simState, 1);
            auto a2s = generateLegalActions(simState, 2);

            for (const auto& a1 : a1s) {
                float sumScore = 0.0f; int visits = 0;
                for (const auto& child : node->children) {
                    if (child->p1Action.type == a1.type && child->p1Action.index == a1.index) {
                        sumScore += (playerId == 1) ? child->winScore : (child->visits - child->winScore);
                        visits += child->visits;
                    }
                }
                float ucb = visits == 0 ? 9999.0f : (sumScore / visits) + 1.41f * std::sqrt(std::log((float)node->visits) / visits);
                if (ucb > bestUCB1) { bestUCB1 = ucb; bestA1 = a1; }
            }
            
            for (const auto& a2 : a2s) {
                float sumScore = 0.0f; int visits = 0;
                for (const auto& child : node->children) {
                    if (child->p2Action.type == a2.type && child->p2Action.index == a2.index) {
                        sumScore += (playerId == 2) ? child->winScore : (child->visits - child->winScore);
                        visits += child->visits;
                    }
                }
                float ucb = visits == 0 ? 9999.0f : (sumScore / visits) + 1.41f * std::sqrt(std::log((float)node->visits) / visits);
                if (ucb > bestUCB2) { bestUCB2 = ucb; bestA2 = a2; }
            }

            for (const auto& child : node->children) {
                if (child->p1Action.type == bestA1.type && child->p1Action.index == bestA1.index &&
                    child->p2Action.type == bestA2.type && child->p2Action.index == bestA2.index) {
                    bestChild = child.get();
                    break;
                }
            }
            
            if (!bestChild) break; 
            node = bestChild;
            simState.step(node->p1Action, node->p2Action);
        }

        if (simState.player1.hasAlivePokemon() && simState.player2.hasAlivePokemon()) {
            auto a1s = generateLegalActions(simState, 1);
            auto a2s = generateLegalActions(simState, 2);
            
            bool expanded = false;
            for (const auto& a1 : a1s) {
                for (const auto& a2 : a2s) {
                    bool exists = false;
                    for (const auto& child : node->children) {
                        if (child->p1Action.type == a1.type && child->p1Action.index == a1.index &&
                            child->p2Action.type == a2.type && child->p2Action.index == a2.index) {
                            exists = true; break;
                        }
                    }
                    if (!exists) {
                        node->children.push_back(std::make_unique<MCTSNode>(node, a1, a2));
                        node = node->children.back().get();
                        simState.step(a1, a2);
                        expanded = true;
                        break;
                    }
                }
                if (expanded) break;
            }
        }

        float win = simulateRandomPlayout(simState, playerId);

        while (node != nullptr) {
            node->visits++;
            node->winScore += win;
            node = node->parent;
        }
    }

    int bestVisits = -1;
    Action bestAction = myActions[0];
    
    for (const auto& action : myActions) {
        int visits = 0;
        for (const auto& child : root->children) {
            if (playerId == 1 && child->p1Action.type == action.type && child->p1Action.index == action.index) visits += child->visits;
            if (playerId == 2 && child->p2Action.type == action.type && child->p2Action.index == action.index) visits += child->visits;
        }
        if (visits > bestVisits) {
            bestVisits = visits;
            bestAction = action;
        }
    }

    return bestAction;
}
