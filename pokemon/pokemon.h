#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "typing.h"
#include "move.h"

class Pokemon
{
public:
    //the statistics, or "stats" of a pokemon
    //determines how much health, how much damage etc
    struct Stats
    {
        int HP;
        int ATK;
        int DEF;
        int SPATK;
        int SPDEF;
        int SPD;
    };

    //in a battle, moves can temporarily raise/lower some stats
    //additionally, theres also Accuracy and Evasion
    //stat modifiers go from -6 to 6, with 0 as default
    std::map<ModifiableStats,int> statModifiers;

private:

    //persistent stats
    std::string name;
    unsigned level;
    Typing primary;
    Typing secondary;
    Stats baseStats;

    std::vector<Move> moveset; //MAX 4

    //stats that can be modified in battle
    //actual stats used in battle, calculated from base state
    //RealStat = BaseStat + LV/50*BaseStat;
    Stats realStat;
    int remainingHP;
    //to limit the scope, the Iv/EV system and Nature system is not included
    StatusCondition statusCondition;
    bool fainted;

public:
    void setName(std::string n) { name = n; }
    std::string getName() { return name; }

    void setLevel(int lv) { level = lv; }
    int getLevel() { return level; }

    void setTyping(Typing p, Typing s)
    {
        primary = p;
        secondary = s;
    }
    Typing getPrimaryType() { return primary; }
    Typing getSecondaryType() { return secondary; }

    void setBaseStats(int hp, int atk, int def, int spatk, int spdef, int spd)
    {
        baseStats = Stats{hp, atk, def, spatk, spdef, spd};
    }

    int getAtk() { return realStat.ATK; }
    int getDef() { return realStat.DEF; }
    int getSpAtk() { return realStat.SPATK; }
    int getSpDef() { return realStat.SPDEF; }
    int getSpd() { return realStat.SPD; }

    int getRemainingHP() { return remainingHP; }
    void heal(int heal) { remainingHP = std::min(realStat.HP, remainingHP+heal); }
    void takeDamage(int dmg)
    {
        remainingHP -= dmg;
        if (remainingHP <= 0) fainted = true;
    }

    void setStatusCondition(StatusCondition sc) { statusCondition = sc; }

    void battleInitialize()
    {
        //set up effective stats
        realStat.HP = (2*baseStats.HP*level)/100 + level + 10;

        realStat.ATK = (2*baseStats.ATK*level)/100 + 5;
        realStat.DEF = (2*baseStats.DEF*level)/100 + 5;
        realStat.SPATK = (2*baseStats.SPATK*level)/100 + 5;
        realStat.SPDEF = (2*baseStats.SPDEF*level)/100 + 5;
        realStat.SPD = (2*baseStats.SPD*level)/100 + 5;

        remainingHP = realStat.HP;
        statusCondition = StatusCondition::NONE;

        statModifiers[ModifiableStats::ATK] = 0;
        statModifiers[ModifiableStats::DEF] = 0;
        statModifiers[ModifiableStats::SPATK] = 0;
        statModifiers[ModifiableStats::SPDEF] = 0;
        statModifiers[ModifiableStats::SPD] = 0;
        statModifiers[ModifiableStats::ACC] = 0;
        statModifiers[ModifiableStats::EVA] = 0;

        for (Move& m : moveset)
        {
            m.resetPP();
        }

    }
};

