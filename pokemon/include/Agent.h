#pragma once
#include "GameState.h"

class Agent {
public:
    virtual Action getAction(const GameState& state, int playerId) = 0;
    virtual ~Agent() = default;
};
