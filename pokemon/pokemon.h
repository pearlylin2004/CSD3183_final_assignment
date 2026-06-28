#pragma once
#include <string>
#include <vector>
#include "Move.h"
#include "Types.h"

class Pokemon {
public:
    std::string name;
    Type type1;
    Type type2;
    int level;
    int base_hp, base_atk, base_def, base_spd;
    int iv_hp, iv_atk, iv_def, iv_spd;

    int max_hp;
    int current_hp;
    int attack;
    int defense;
    int speed;
    std::vector<Move> moves;

    struct LearnMove {
        int level;
        Move move;
    };
    std::vector<LearnMove> learnset;

    Pokemon(std::string n, Type t1, Type t2, int b_hp, int b_atk, int b_def, int b_spd);

    void setLevel(int newLevel);

    void addMove(Move m);
    void addLearnMove(int lvl, Move m);
    bool isAlive() const;
    void takeDamage(int damage);
    void heal(int amount);
};
