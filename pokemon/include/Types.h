#pragma once
#include <string>

enum class Type {
    Normal, Fire, Water, Grass, Electric, Ice, Fighting, Poison, Ground, 
    Flying, Psychic, Bug, Rock, Ghost, Dragon, Dark, Steel, Fairy, None
};

inline std::string typeToString(Type t) {
    switch(t) {
        case Type::Normal: return "Normal";
        case Type::Fire: return "Fire";
        case Type::Water: return "Water";
        case Type::Grass: return "Grass";
        case Type::Electric: return "Electric";
        case Type::Ice: return "Ice";
        case Type::Fighting: return "Fighting";
        case Type::Poison: return "Poison";
        case Type::Ground: return "Ground";
        case Type::Flying: return "Flying";
        case Type::Psychic: return "Psychic";
        case Type::Bug: return "Bug";
        case Type::Rock: return "Rock";
        case Type::Ghost: return "Ghost";
        case Type::Dragon: return "Dragon";
        case Type::Dark: return "Dark";
        case Type::Steel: return "Steel";
        case Type::Fairy: return "Fairy";
        default: return "None";
    }
}

inline float getTypeEffectiveness(Type attackType, Type defendType) {
    if (attackType == Type::Normal) {
        if (defendType == Type::Rock || defendType == Type::Steel) return 0.5f;
        if (defendType == Type::Ghost) return 0.0f;
    }
    else if (attackType == Type::Fire) {
        if (defendType == Type::Grass || defendType == Type::Ice || defendType == Type::Bug || defendType == Type::Steel) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Water || defendType == Type::Rock || defendType == Type::Dragon) return 0.5f;
    }
    else if (attackType == Type::Water) {
        if (defendType == Type::Fire || defendType == Type::Ground || defendType == Type::Rock) return 2.0f;
        if (defendType == Type::Water || defendType == Type::Grass || defendType == Type::Dragon) return 0.5f;
    }
    else if (attackType == Type::Grass) {
        if (defendType == Type::Water || defendType == Type::Ground || defendType == Type::Rock) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Grass || defendType == Type::Poison || defendType == Type::Flying || defendType == Type::Bug || defendType == Type::Dragon || defendType == Type::Steel) return 0.5f;
    }
    else if (attackType == Type::Electric) {
        if (defendType == Type::Water || defendType == Type::Flying) return 2.0f;
        if (defendType == Type::Electric || defendType == Type::Grass || defendType == Type::Dragon) return 0.5f;
        if (defendType == Type::Ground) return 0.0f;
    }
    else if (attackType == Type::Ice) {
        if (defendType == Type::Grass || defendType == Type::Ground || defendType == Type::Flying || defendType == Type::Dragon) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Water || defendType == Type::Ice || defendType == Type::Steel) return 0.5f;
    }
    else if (attackType == Type::Fighting) {
        if (defendType == Type::Normal || defendType == Type::Ice || defendType == Type::Rock || defendType == Type::Dark || defendType == Type::Steel) return 2.0f;
        if (defendType == Type::Poison || defendType == Type::Flying || defendType == Type::Psychic || defendType == Type::Bug || defendType == Type::Fairy) return 0.5f;
        if (defendType == Type::Ghost) return 0.0f;
    }
    else if (attackType == Type::Poison) {
        if (defendType == Type::Grass || defendType == Type::Fairy) return 2.0f;
        if (defendType == Type::Poison || defendType == Type::Ground || defendType == Type::Rock || defendType == Type::Ghost) return 0.5f;
        if (defendType == Type::Steel) return 0.0f;
    }
    else if (attackType == Type::Ground) {
        if (defendType == Type::Fire || defendType == Type::Electric || defendType == Type::Poison || defendType == Type::Rock || defendType == Type::Steel) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Bug) return 0.5f;
        if (defendType == Type::Flying) return 0.0f;
    }
    else if (attackType == Type::Flying) {
        if (defendType == Type::Grass || defendType == Type::Fighting || defendType == Type::Bug) return 2.0f;
        if (defendType == Type::Electric || defendType == Type::Rock || defendType == Type::Steel) return 0.5f;
    }
    else if (attackType == Type::Psychic) {
        if (defendType == Type::Fighting || defendType == Type::Poison) return 2.0f;
        if (defendType == Type::Psychic || defendType == Type::Steel) return 0.5f;
        if (defendType == Type::Dark) return 0.0f;
    }
    else if (attackType == Type::Bug) {
        if (defendType == Type::Grass || defendType == Type::Psychic || defendType == Type::Dark) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Fighting || defendType == Type::Poison || defendType == Type::Flying || defendType == Type::Ghost || defendType == Type::Steel || defendType == Type::Fairy) return 0.5f;
    }
    else if (attackType == Type::Rock) {
        if (defendType == Type::Fire || defendType == Type::Ice || defendType == Type::Flying || defendType == Type::Bug) return 2.0f;
        if (defendType == Type::Fighting || defendType == Type::Ground || defendType == Type::Steel) return 0.5f;
    }
    else if (attackType == Type::Ghost) {
        if (defendType == Type::Psychic || defendType == Type::Ghost) return 2.0f;
        if (defendType == Type::Dark) return 0.5f;
        if (defendType == Type::Normal) return 0.0f;
    }
    else if (attackType == Type::Dragon) {
        if (defendType == Type::Dragon) return 2.0f;
        if (defendType == Type::Steel) return 0.5f;
        if (defendType == Type::Fairy) return 0.0f;
    }
    else if (attackType == Type::Dark) {
        if (defendType == Type::Psychic || defendType == Type::Ghost) return 2.0f;
        if (defendType == Type::Fighting || defendType == Type::Dark || defendType == Type::Fairy) return 0.5f;
    }
    else if (attackType == Type::Steel) {
        if (defendType == Type::Ice || defendType == Type::Rock || defendType == Type::Fairy) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Water || defendType == Type::Electric || defendType == Type::Steel) return 0.5f;
    }
    else if (attackType == Type::Fairy) {
        if (defendType == Type::Fighting || defendType == Type::Dragon || defendType == Type::Dark) return 2.0f;
        if (defendType == Type::Fire || defendType == Type::Poison || defendType == Type::Steel) return 0.5f;
    }
    return 1.0f;
}
