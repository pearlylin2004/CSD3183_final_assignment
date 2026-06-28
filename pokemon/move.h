#pragma once
#include <string>
#include "Types.h"

enum class Stat { None, Attack, Defense, Speed, Accuracy, Evasion };

struct Move {
    std::string name;
    Type type;
    int power;
    int accuracy; // 0 to 100
    Stat statTarget = Stat::None;
    int statChange = 0;
    bool targetSelf = false;
};
