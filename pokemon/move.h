#include <string>
#include <vector>
#include "typing.h"

enum StatusCondition
{
    NONE =0,
    BURN,
    FREEZE,
    PARALYSE,
    POISON,
    TOXIC,
    SLEEP
};
enum ModifiableStats
{
    ATK,DEF,SPATK,SPDEF,SPD,EVA,ACC
};
class Move
{
    public: 
    //used to determine which ATK and DEf stats are used for calculations
    enum Category
    {
        PHYSICAL = 0,
        SPECIAL,
        STATUS
    };

    //flags for special types of moves
    //e.g contact moves
    enum Flag
    {
        //TBA
    };

    private:
    std::string name;
    Typing type;
    Category category;

    //move data
    int Power;
    int PP; //how many times a move can be used
    int remainingPP; //remaining uses
    float Accuracy;

    // what status condition the move can inflict
    StatusCondition inflictsStatus;
    // what stats the move can modify on the enemy, empty if none
    std::vector<std::pair<ModifiableStats,int>> modifyEnemyStats;
    // what stats the move can modify on self
    std::vector<std::pair<ModifiableStats,int>> modifySelfStats; 

    std::vector<Flag> flags;

    public:
    void resetPP() { remainingPP = PP; }
};