#include "GreedyAgent.h"

Action GreedyAgent::getAction(const GameState& state, int playerId) {
    const Trainer& ally = (playerId == 1) ? state.player1 : state.player2;
    const Trainer& enemy = (playerId == 1) ? state.player2 : state.player1;

    const Pokemon* allyActive = ally.getActivePokemon();
    const Pokemon* enemyActive = enemy.getActivePokemon();

    // Forced switch if active pokemon is dead
    if (!allyActive) {
        for (int i = 0; i < ally.party.size(); ++i) {
            if (ally.party[i].isAlive()) {
                return {ActionType::SWITCH, i};
            }
        }
        return {ActionType::MOVE, 0}; // Fallback if entire party is dead
    }
    
    // Evaluate moves to find the one that does the most damage
    int bestMoveIndex = 0;
    float bestScore = -9999.0f;
    
    for (int i = 0; i < allyActive->moves.size(); ++i) {
        const Move& move = allyActive->moves[i];
        
        float score = 0.0f;
        if (move.power > 0) {
            float stab = (allyActive->type1 == move.type || allyActive->type2 == move.type) ? 1.5f : 1.0f;
            float effectiveness = 1.0f;
            if (enemyActive) {
                effectiveness = getTypeEffectiveness(move.type, enemyActive->type1) * getTypeEffectiveness(move.type, enemyActive->type2);
            }
            // Estimate average expected damage factoring in acc
            score = (move.power * stab * effectiveness) * (move.accuracy / 100.0f);
        } else if (move.statTarget != Stat::None) {
            // Give a flat nominal score to status moves so if all attacks are bad it uses non attack stuff
            score = 10.0f * (move.accuracy / 100.0f);
        }
        
        if (score > bestScore) {
            bestScore = score;
            bestMoveIndex = i;
        }
    }
    
    // Potions fallback if low HP
    if (ally.potions > 0 && allyActive->current_hp <= allyActive->max_hp * 0.3f) {
        return {ActionType::POTION, 0};
    }

    return {ActionType::MOVE, bestMoveIndex};
}
