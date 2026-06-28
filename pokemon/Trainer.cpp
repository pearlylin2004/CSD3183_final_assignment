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
    for (auto& p : party) {
        if (p.isAlive()) return &p;
    }
    return nullptr;
}

bool Trainer::switchPokemon(int index) {
    if (index >= 0 && index < party.size() && party[index].isAlive()) {
        if (&party[index] == getActivePokemon()) {
            return false; // Cannot switch to the pokemon that is already active
        }
        // Swap selected pokemon to the front (index 0) to make it active
        std::swap(party[0], party[index]);
        return true;
    }
    return false;
}
