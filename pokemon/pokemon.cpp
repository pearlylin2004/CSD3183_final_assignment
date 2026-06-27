#include "pokemon.h"


void foo()
{
    Pokemon example;
    example.setName("Pikachu");
    example.setLevel(55);
    example.setBaseStats(35, 55, 40, 50, 50, 90);
    example.setTyping(Typing::ELECTRIC, Typing::MONO);
}