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

void GameState::simulateMoveHeadless(Pokemon* attacker, Pokemon* defender, Move move, int& defHp, bool& missed, int forcedHit, float& outProb, bool& consumed) {
    missed = false;
    consumed = true;
    
    float accMult = attacker->getAccuracyMultiplier();
    float evaMult = defender->getEvasionMultiplier();
    float hitChanceFloat = (float)move.accuracy * (accMult / evaMult);
    if (hitChanceFloat > 100.0f) hitChanceFloat = 100.0f;
    if (hitChanceFloat < 0.0f) hitChanceFloat = 0.0f;
    
    bool isHit = false;
    if (forcedHit == 1) {
        isHit = true;
        outProb = hitChanceFloat / 100.0f;
    } else if (forcedHit == -1) {
        isHit = false;
        outProb = 1.0f - (hitChanceFloat / 100.0f);
    } else {
        int hitChance = std::rand() % 100 + 1;
        isHit = (hitChance <= hitChanceFloat);
        outProb = 1.0f;
    }

    if (isHit) {
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

int GameState::stepInternal(Action p1Action, Action p2Action, int forcedP1Hit, int forcedP2Hit, float& outProb, bool& p1Consumed, bool& p2Consumed) {
    outProb = 1.0f;
    p1Consumed = false;
    p2Consumed = false;
    
    Pokemon* p1Mon = player1.getActivePokemon();
    Pokemon* p2Mon = player2.getActivePokemon();
    
    if (!p1Mon && p1Action.type != ActionType::SWITCH) return 0;
    if (!p2Mon && p2Action.type != ActionType::SWITCH) return 0;

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

    if (p1Action.type == ActionType::POTION) {
        simulatePotionHeadless(player1, p1Mon, p1SimHp);
    }
    if (p2Action.type == ActionType::POTION) {
        simulatePotionHeadless(player2, p2Mon, p2SimHp);
    }

    bool p1UsedMove = (p1Action.type == ActionType::MOVE);
    bool p2UsedMove = (p2Action.type == ActionType::MOVE);
    
    if (p1UsedMove && p2UsedMove) {
        bool p1GoesFirst = p1Mon->getSpeed() >= p2Mon->getSpeed();
        if (p1GoesFirst) {
            bool missed;
            float prob = 1.0f;
            simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed, forcedP1Hit, prob, p1Consumed);
            outProb *= prob;
            if (p2SimHp > 0) {
                simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed, forcedP2Hit, prob, p2Consumed);
                outProb *= prob;
            }
        } else {
            bool missed;
            float prob = 1.0f;
            simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed, forcedP2Hit, prob, p2Consumed);
            outProb *= prob;
            if (p1SimHp > 0) {
                simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed, forcedP1Hit, prob, p1Consumed);
                outProb *= prob;
            }
        }
    } else if (p1UsedMove) {
        bool missed;
        float prob = 1.0f;
        simulateMoveHeadless(p1Mon, p2Mon, p1Mon->moves[p1Action.index], p2SimHp, missed, forcedP1Hit, prob, p1Consumed);
        outProb *= prob;
    } else if (p2UsedMove) {
        bool missed;
        float prob = 1.0f;
        simulateMoveHeadless(p2Mon, p1Mon, p2Mon->moves[p2Action.index], p1SimHp, missed, forcedP2Hit, prob, p2Consumed);
        outProb *= prob;
    }

    if (!player1.hasAlivePokemon()) return 2;
    if (!player2.hasAlivePokemon()) return 1;

    return 0;
}

int GameState::step(Action p1Action, Action p2Action) {
    float prob = 1.0f;
    bool c1, c2;
    return stepInternal(p1Action, p2Action, 0, 0, prob, c1, c2);
}

std::vector<std::pair<GameState, float>> GameState::stepExpecti(Action p1Action, Action p2Action) const {
    std::vector<std::pair<GameState, float>> outcomes;
    
    bool p1UsedMove = (p1Action.type == ActionType::MOVE);
    bool p2UsedMove = (p2Action.type == ActionType::MOVE);

    int p1Cases = p1UsedMove ? 2 : 1;
    int p2Cases = p2UsedMove ? 2 : 1;

    for (int i = 0; i < p1Cases; ++i) {
        for (int j = 0; j < p2Cases; ++j) {
            int forcedP1Hit = p1UsedMove ? (i == 0 ? 1 : -1) : 0;
            int forcedP2Hit = p2UsedMove ? (j == 0 ? 1 : -1) : 0;

            GameState nextState = *this;
            float prob = 1.0f;
            bool p1Consumed = false;
            bool p2Consumed = false;
            nextState.stepInternal(p1Action, p2Action, forcedP1Hit, forcedP2Hit, prob, p1Consumed, p2Consumed);
            
            if (p1UsedMove && !p1Consumed && i > 0) continue;
            if (p2UsedMove && !p2Consumed && j > 0) continue;

            if (prob > 0.0f) {
                outcomes.push_back({nextState, prob});
            }
        }
    }
    return outcomes;
}

float GameState::evaluate(int perspectivePlayerId) const {
    const Trainer& ally = (perspectivePlayerId == 1) ? player1 : player2;
    const Trainer& enemy = (perspectivePlayerId == 1) ? player2 : player1;
    
    float score = 0.0f;

    float allyHp = 0.0f;
    for (const auto& mon : ally.party) {
        if (mon.isAlive()) allyHp += (float)mon.current_hp;
    }
    
    float enemyHp = 0.0f;
    for (const auto& mon : enemy.party) {
        if (mon.isAlive()) enemyHp += (float)mon.current_hp;
    }
    
    score += (allyHp - enemyHp);

    score += (ally.potions - enemy.potions) * 18.0f;

    return score;
}
