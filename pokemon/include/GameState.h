#pragma once
#include "Trainer.h"

enum class ActionType {
    MOVE,
    SWITCH,
    POTION
};

struct Action {
    ActionType type;
    int index; // 0-3 for MOVE, 1-5 for SWITCH, 0 for POTION
};

class GameState {
public:
    Trainer player1;
    Trainer player2;

    GameState(Trainer p1, Trainer p2);

    // Simulates a turn given both players' actions.
    // Returns 1 if player1 wins, 2 if player2 wins, 0 if ongoing.
    int step(Action p1Action, Action p2Action);
    
    // Evaluate function as per heuristic specification
    float evaluate(int perspectivePlayer) const;

private:
    void simulateMoveHeadless(Pokemon* attacker, Pokemon* defender, Move move, int& defHp, bool& missed);
    void simulatePotionHeadless(Trainer& trainer, Pokemon* mon, int& simHp);
};
