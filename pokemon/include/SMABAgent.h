#pragma once
#include "Agent.h"
#include <vector>

class SMABAgent : public Agent {
public:
    int searchDepth;

    SMABAgent(int depth = 2);
    Action getAction(const GameState& state, int playerId) override;

private:
    std::vector<Action> generateLegalActions(const GameState& state, int playerId);
    float smabSearch(const GameState& state, int depth, int povPlayerId);
};
