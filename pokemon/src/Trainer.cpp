#include "Trainer.h"

Trainer::Trainer(std::string n, int potionCount) : name(n), potions(potionCount) {}

void Trainer::addPokemon(Pokemon p) {
    if (party.size() < 6) {
        party.push_back(p);
    }
}

bool Trainer::hasAlivePokemon() const {
    for (const auto& p : party) {
        if (p.isAlive()) return true;
    }
    return false;
}

void Trainer::healAll() {
    for (auto& p : party) {
        p.heal(9999); // max heal
    }
    potions = 3; // Reset potions for next battle
}

Pokemon* Trainer::getActivePokemon() {
    if (!party.empty() && party[0].isAlive()) {
        return &party[0];
    }
    return nullptr;
}

const Pokemon* Trainer::getActivePokemon() const {
    if (!party.empty() && party[0].isAlive()) {
        return &party[0];
    }
    return nullptr;
}

bool Trainer::switchPokemon(int index) {
    if (index >= 0 && index < party.size() && party[index].isAlive()) {
        if (index == 0) { // Wait, the old code swapped, so active is always at 0
            return false;
        }
        std::swap(party[0], party[index]);
        party[0].resetStages(); // Reset stages for the incoming pokemon
        return true;
    }
    return false;
}
