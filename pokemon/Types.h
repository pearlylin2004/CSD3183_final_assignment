#pragma once
#include <string>

enum class Type {
    Normal,
    Fire,
    Water,
    Grass,
    Electric,
    Psychic,
    None
};

inline std::string typeToString(Type t) {
    switch(t) {
        case Type::Normal: return "Normal";
        case Type::Fire: return "Fire";
        case Type::Water: return "Water";
        case Type::Grass: return "Grass";
        case Type::Electric: return "Electric";
        case Type::Psychic: return "Psychic";
        default: return "None";
    }
}

inline float getTypeEffectiveness(Type attackType, Type defendType) {
    if (attackType == Type::Fire) {
        if (defendType == Type::Grass) return 2.0f;
        if (defendType == Type::Water || defendType == Type::Fire) return 0.5f;
    }
    else if (attackType == Type::Water) {
        if (defendType == Type::Fire) return 2.0f;
        if (defendType == Type::Water || defendType == Type::Grass) return 0.5f;
    }
    else if (attackType == Type::Grass) {
        if (defendType == Type::Water) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Fire) return 0.5f;
    }
    else if (attackType == Type::Electric) {
        if (defendType == Type::Water) return 2.0f;
        if (defendType == Type::Grass || defendType == Type::Electric) return 0.5f;
    }
    else if (attackType == Type::Psychic) {
        if (defendType == Type::Psychic) return 0.5f;
        // In gen 1, Psychic has no weaknesses and is super effective against Poison/Fighting (which aren't added yet)
    }
    return 1.0f;
}
