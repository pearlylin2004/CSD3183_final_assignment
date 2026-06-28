#pragma once
#include <string>
#include <vector>
#include "Pokemon.h"

class Trainer {
public:
    std::string name;
    std::vector<Pokemon> party;
    int potions;

    Trainer(std::string n, int potionCount = 3);
    void addPokemon(Pokemon p);
    void healAll();
    bool hasAlivePokemon() const;
    Pokemon* getActivePokemon();
    bool switchPokemon(int index);
};
