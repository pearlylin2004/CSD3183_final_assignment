#pragma once
#include "Agent.h"
#include <vector>
#include <memory>
#include <map>

struct MCTSNode {
    int visits;
    float winScore;
    Action p1Action;
    Action p2Action;
    std::vector<std::unique_ptr<MCTSNode>> children;
    MCTSNode* parent;

    MCTSNode(MCTSNode* parent = nullptr, Action a1 = {ActionType::MOVE, 0}, Action a2 = {ActionType::MOVE, 0});
    bool isFullyExpanded(int numP1Actions, int numP2Actions) const;
};

class MCTSAgent : public Agent {
public:
    int iterations;

    MCTSAgent(int iters = 1000);
    Action getAction(const GameState& state, int playerId) override;

private:
    std::vector<Action> generateLegalActions(const GameState& state, int playerId);
    GameState determinize(const GameState& state, int playerId);
    int simulateRandomPlayout(GameState state, int povPlayerId);
};
