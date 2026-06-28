#include "Pokemon.h"
#include <cstdlib>
#include <cmath>

Pokemon::Pokemon(std::string n, Type t1, Type t2, int b_hp, int b_atk, int b_def, int b_spd)
    : name(n), type1(t1), type2(t2), base_hp(b_hp), base_atk(b_atk), base_def(b_def), base_spd(b_spd), level(5) {
    // Generate random IVs (0-31)
    iv_hp = std::rand() % 32;
    iv_atk = std::rand() % 32;
    iv_def = std::rand() % 32;
    iv_spd = std::rand() % 32;

    setLevel(5); // Default to lvl 5
}

void Pokemon::setLevel(int newLevel) {
    level = newLevel;
    // Calculate stats using Gen 3+ formula (excluding EVs cos fk that)
    max_hp = static_cast<int>(std::floor(0.01f * (2.0f * base_hp + iv_hp) * level)) + level + 10;
    attack = static_cast<int>(std::floor(0.01f * (2.0f * base_atk + iv_atk) * level)) + 5;
    defense = static_cast<int>(std::floor(0.01f * (2.0f * base_def + iv_def) * level)) + 5;
    speed = static_cast<int>(std::floor(0.01f * (2.0f * base_spd + iv_spd) * level)) + 5;
    
    current_hp = max_hp;

    moves.clear();
    for (const auto& lm : learnset) {
        if (lm.level <= level) {
            if (moves.size() == 4) {
                moves.erase(moves.begin());
            }
            moves.push_back(lm.move);
        }
    }
}

void Pokemon::addLearnMove(int lvl, Move m) {
    learnset.push_back({lvl, m});
    // Re-apply in case level is already set
    if (lvl <= level) {
        if (moves.size() == 4) {
            moves.erase(moves.begin());
        }
        moves.push_back(m);
    }
}

void Pokemon::addMove(Move m) {
    if (moves.size() == 4) {
        moves.erase(moves.begin());
    }
    moves.push_back(m);
}

bool Pokemon::isAlive() const {
    return current_hp > 0;
}

void Pokemon::takeDamage(int damage) {
    current_hp -= damage;
    if (current_hp < 0) current_hp = 0;
}

void Pokemon::heal(int amount) {
    current_hp += amount;
    if (current_hp > max_hp) current_hp = max_hp;
}

void Pokemon::resetStages() {
    attack_stage = 0;
    defense_stage = 0;
    speed_stage = 0;
    accuracy_stage = 0;
    evasion_stage = 0;
}

static float getStatMultiplier(int stage) {
    if (stage > 0) return (2.0f + stage) / 2.0f;
    if (stage < 0) return 2.0f / (2.0f - stage);
    return 1.0f;
}

static float getAccEvaMultiplier(int stage) {
    if (stage > 0) return (3.0f + stage) / 3.0f;
    if (stage < 0) return 3.0f / (3.0f - stage);
    return 1.0f;
}

int Pokemon::getAttack() const {
    return static_cast<int>(attack * getStatMultiplier(attack_stage));
}

int Pokemon::getDefense() const {
    return static_cast<int>(defense * getStatMultiplier(defense_stage));
}

int Pokemon::getSpeed() const {
    return static_cast<int>(speed * getStatMultiplier(speed_stage));
}

float Pokemon::getAccuracyMultiplier() const {
    return getAccEvaMultiplier(accuracy_stage);
}

float Pokemon::getEvasionMultiplier() const {
    return getAccEvaMultiplier(evasion_stage);
}
