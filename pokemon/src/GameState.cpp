#include "GameState.h"
#include "Types.h"
#include <algorithm>
#include <cstdlib>

GameState::GameState(Trainer p1, Trainer p2) : player1(p1), player2(p2) {}

void GameState::simulatePotionHeadless(Trainer& trainer, Pokemon* mon, int& simHp) {
    if (trainer.potions > 0) {
        trainer.potions--;
        int healed = std::min(20, mon->max_hp - simHp);
        simHp += healed;
        mon->heal(20);
    }
}

void GameState::simulateMoveHeadless(Pokemon* attacker, Pokemon* defender, Move move, int& defHp, bool& missed) {
    missed = false;
    int hitChance = std::rand() % 100 + 1;
    float accMult = attacker->getAccuracyMultiplier();
    float evaMult = defender->getEvasionMultiplier();
    
    if (hitChance <= move.accuracy * (accMult / evaMult)) {
        if (move.power > 0) {
            float stab = (attacker->type1 == move.type || attacker->type2 == move.type) ? 1.5f : 1.0f;
            float effectiveness = getTypeEffectiveness(move.type, defender->type1) * getTypeEffectiveness(move.type, defender->type2);
            
            int damage = static_cast<int>((((2.0f * 5.0f / 5.0f + 2.0f) * move.power * ((float)attacker->getAttack() / defender->getDefense())) / 50.0f + 2.0f) * stab * effectiveness);
            if (damage < 1) damage = 1;
            
            defHp -= damage;
            defender->takeDamage(damage);
        } else {
            if (move.statTarget != Stat::None) {
                Pokemon* statTargetMon = move.targetSelf ? attacker : defender;
                int* stagePtr = nullptr;
                if (move.statTarget == Stat::Attack) stagePtr = &statTargetMon->attack_stage;
                else if (move.statTarget == Stat::Defense) stagePtr = &statTargetMon->defense_stage;
                else if (move.statTarget == Stat::Speed) stagePtr = &statTargetMon->speed_stage;
                else if (move.statTarget == Stat::Accuracy) stagePtr = &statTargetMon->accuracy_stage;
                else if (move.statTarget == Stat::Evasion) stagePtr = &statTargetMon->evasion_stage;
                
                if (stagePtr) {
                    *stagePtr += move.statChange;
                    if (*stagePtr > 6) *stagePtr = 6;
                    if (*stagePtr < -6) *stagePtr = -6;
                }
            }
        }
    } else {
        missed = true;
    }
}

int GameState::step(Action p1Action, Action p2Action) {
    Pokemon* p1Mon = player1.getActivePokemon();
    Pokemon* p2Mon = player2.getActivePokemon();
    
    // In force switch scenario
    if (!p1Mon && p1Action.type != ActionType::SWITCH) return 0;
    if (!p2Mon && p2Action.type != ActionType::SWITCH) return 0;

    // 1. Handle switches first (Switching has highest priority)
    if (p1Action.type == ActionType::SWITCH) {
        player1.switchPokemon(p1Action.index);
        p1Mon = player1.getActivePokemon();
    }
    if (p2Action.type == ActionType::SWITCH) {
        player2.switchPokemon(p2Action.index);
        p2Mon = player2.getActivePokemon();
    }

    int p1SimHp = p1Mon->current_hp;
    int p2SimHp = p2Mon->current_hp;

    // 2. Handle potions (Items have next priority)
    if (p1Action.type == ActionType::POTION) {
        simulatePotionHeadless(player1, p1Mon, p1SimHp);
    }
    if (p2Action.type == ActionType::POTION) {
        simulatePotionHeadless(player2, p2Mon, p2SimHp);
    }

    // 3. Handle moves based on speed
    bool p1UsedMove = (p1Action.type == ActionType::MOVE);
    bool p2UsedMove = (p2Action.type == ActionType::MOVE);
    
    if (p1UsedMove && p2UsedMove) {
        bool p1GoesFirst = p1Mon->getSpeed() >= p2Mon->getSpeed();
        if (p1GoesFirst) {
            bool missed;
            simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed);
            if (p2SimHp > 0) {
                simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed);
            }
        } else {
            bool missed;
            simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed);
            if (p1SimHp > 0) {
                simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed);
            }
        }
    } else if (p1UsedMove) {
        bool missed;
        simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed);
    } else if (p2UsedMove) {
        bool missed;
        simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed);
    }

    // 4. Check win conditions
    if (!player1.hasAlivePokemon()) return 2;
    if (!player2.hasAlivePokemon()) return 1;

    return 0; // ongoing
}

float GameState::evaluate(int perspectivePlayerId) const {
    const Trainer& ally = (perspectivePlayerId == 1) ? player1 : player2;
    const Trainer& enemy = (perspectivePlayerId == 1) ? player2 : player1;
    
    float score = 0.0f;

    // 1. HP Differential (Massive weight)
    float allyHpPerc = 0.0f;
    for (const auto& mon : ally.party) {
        if (mon.max_hp > 0) allyHpPerc += (float)mon.current_hp / mon.max_hp;
    }
    
    float enemyHpPerc = 0.0f;
    for (const auto& mon : enemy.party) {
        if (mon.max_hp > 0) enemyHpPerc += (float)mon.current_hp / mon.max_hp;
    }
    
    score += (allyHpPerc - enemyHpPerc) * 100.0f; // Scale to 100 points per full pokemon HP bar

    // 2. Stat Modifiers
    const Pokemon* allyActive = ally.getActivePokemon();
    if (allyActive) {
        score += allyActive->attack_stage * 10.0f;
        score += allyActive->defense_stage * 10.0f;
        score += allyActive->speed_stage * 10.0f;
        score += allyActive->accuracy_stage * 5.0f;
        score += allyActive->evasion_stage * 5.0f;
    }
    
    const Pokemon* enemyActive = enemy.getActivePokemon();
    if (enemyActive) {
        score -= enemyActive->attack_stage * 10.0f;
        score -= enemyActive->defense_stage * 10.0f;
        score -= enemyActive->speed_stage * 10.0f;
        score -= enemyActive->accuracy_stage * 5.0f;
        score -= enemyActive->evasion_stage * 5.0f;
        
        // 3. Type coverage heuristic (Offensive advantage)
        if (allyActive) {
            float bestEffectiveness = 0.0f;
            // Does ally have a move that is super effective?
            for (const auto& move : allyActive->moves) {
                if (move.power > 0) {
                    float eff = getTypeEffectiveness(move.type, enemyActive->type1) * getTypeEffectiveness(move.type, enemyActive->type2);
                    if (eff > bestEffectiveness) bestEffectiveness = eff;
                }
            }
            if (bestEffectiveness > 1.0f) score += 20.0f;
            else if (bestEffectiveness < 1.0f) score -= 20.0f;
        }
    }

    // 4. Potions remaining
    score += (ally.potions - enemy.potions) * 15.0f;

    return score;
}
