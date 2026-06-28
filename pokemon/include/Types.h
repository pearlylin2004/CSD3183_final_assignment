#pragma once
#include <string>

enum class Type {
    Normal, Fire, Water, Grass, Electric, Psychic, 
    Bug, Poison, Flying, Dark, Ground, Fairy, Ghost, None
};

inline std::string typeToString(Type t) {
    switch(t) {
        case Type::Normal: return "Normal";
        case Type::Fire: return "Fire";
        case Type::Water: return "Water";
        case Type::Grass: return "Grass";
        case Type::Electric: return "Electric";
        case Type::Psychic: return "Psychic";
        case Type::Bug: return "Bug";
        case Type::Poison: return "Poison";
        case Type::Flying: return "Flying";
        case Type::Dark: return "Dark";
        case Type::Ground: return "Ground";
        case Type::Fairy: return "Fairy";
        case Type::Ghost: return "Ghost";
        default: return "None";
    }
}

inline float getTypeEffectiveness(Type attackType, Type defendType) {
    if (attackType == Type::Normal) {
        if (defendType == Type::Ghost) return 0.0f;
    }
    else if (attackType == Type::Fire) {
        if (defendType == Type::Grass || defendType == Type::Bug) return 2.0f;
        if (defendType == Type::Water || defendType == Type::Fire) return 0.5f;
    }
    else if (attackType == Type::Water) {
        if (defendType == Type::Fire || defendType == Type::Ground) return 2.0f;
        if (defendType == Type::Water || defendType == Type::Grass) return 0.5f;
    }
    else if (attackType == Type::Grass) {
        if (defendType == Type::Water || defendType == Type::Ground) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Fire || defendType == Type::Bug || defendType == Type::Poison || defendType == Type::Flying) return 0.5f;
    }
    else if (attackType == Type::Electric) {
        if (defendType == Type::Water || defendType == Type::Flying) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Electric) return 0.5f;
        if (defendType == Type::Ground) return 0.0f;
    }
    else if (attackType == Type::Psychic) {
        if (defendType == Type::Poison) return 2.0f;
        if (defendType == Type::Psychic) return 0.5f;
        if (defendType == Type::Dark) return 0.0f;
    }
    else if (attackType == Type::Bug) {
        if (defendType == Type::Grass || defendType == Type::Psychic || defendType == Type::Dark) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Poison || defendType == Type::Flying || defendType == Type::Ghost || defendType == Type::Fairy) return 0.5f;
    }
    else if (attackType == Type::Poison) {
        if (defendType == Type::Grass || defendType == Type::Fairy) return 2.0f;
        if (defendType == Type::Poison || defendType == Type::Ground || defendType == Type::Ghost) return 0.5f;
    }
    else if (attackType == Type::Flying) {
        if (defendType == Type::Grass || defendType == Type::Bug) return 2.0f;
        if (defendType == Type::Electric) return 0.5f;
    }
    else if (attackType == Type::Dark) {
        if (defendType == Type::Psychic || defendType == Type::Ghost) return 2.0f;
        if (defendType == Type::Dark || defendType == Type::Fairy) return 0.5f;
    }
    else if (attackType == Type::Ground) {
        if (defendType == Type::Fire || defendType == Type::Electric || defendType == Type::Poison) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Bug) return 0.5f;
        if (defendType == Type::Flying) return 0.0f;
    }
    else if (attackType == Type::Fairy) {
        if (defendType == Type::Dark) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Poison) return 0.5f;
    }
    else if (attackType == Type::Ghost) {
        if (defendType == Type::Psychic || defendType == Type::Ghost) return 2.0f;
        if (defendType == Type::Dark) return 0.5f;
        if (defendType == Type::Normal) return 0.0f;
    }
    return 1.0f;
}
