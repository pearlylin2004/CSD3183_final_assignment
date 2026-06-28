#pragma once
#include <string>
#include "Types.h"

struct Move {
    std::string name;
    Type type;
    int power;
    int accuracy; // 0 to 100
};
