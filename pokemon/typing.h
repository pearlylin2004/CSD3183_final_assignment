
enum Typing
{
    NORMAL = 0,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK,
    MONO //for monotypes only. not an actual typing
};

float typeChart[18][18] =
{
//   NML   FGT   FLY   POI   GRN   RCK   BUG   GHO   STL   FIR   WTR   GRS   ELC   PSY   ICE   DRA   DRK   MONO
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, //normal
    {2.0f, 1.0f, 0.5f, 0.5f, 1.0f, 2.0f, 0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 0.5f, 2.0f, 1.0f}, //fighting
    {1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 1.0f, 0.5f, 1.0f, 1.0f, 2.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, //flying
    {1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, //poison
    {1.0f, 1.0f, 0.0f, 2.0f, 1.0f, 2.0f, 0.5f, 1.0f, 2.0f, 2.0f, 1.0f, 0.5f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, //ground
    {1.0f, 0.5f, 2.0f, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f, 0.5f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f}, //rock
    {1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 1.0f, 2.0f, 1.0f}, //bug
    {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 0.5f, 1.0f}, //ghost
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f}, //steel
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 1.0f, 2.0f, 0.5f, 0.5f, 2.0f, 1.0f, 1.0f, 2.0f, 0.5f, 1.0f, 1.0f}, //fire
    {1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f}, //water
    {1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 2.0f, 0.5f, 1.0f, 0.5f, 0.5f, 2.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f}, //grass
    {1.0f, 1.0f, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f}, //electric
    {1.0f, 1.0f, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f}, //psychic
    {1.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 2.0f, 1.0f, 1.0f, 0.5f, 2.0f, 1.0f, 1.0f}, //ice
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f}, //dragon
    {1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 0.5f, 1.0f}, //dark
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}  //mono 
};

//EXAMPLE : lookup effectiveness of WATER against FIRE
// typeChart[WATER][FIRE] = 2.0

// effectiveness of a FIRE move against an ICE/WATER pokemon
// typechart[FIRE][ICE] * typechart[FIRE][WATER] = 2.0 * 0.5 = 1.0;

// effectiveness of a NORMAL move against a GHOST pokemon
// typeChart[NORMAL][GHOST] * typechart[NORMAL][MONO] = 0.0 * 1.0 = 0.0;