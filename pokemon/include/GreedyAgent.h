#pragma once
#include "Agent.h"

class GreedyAgent : public Agent {
public:
    Action getAction(const GameState& state, int playerId) override;
};
