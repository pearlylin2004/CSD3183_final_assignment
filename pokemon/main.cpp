#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include "Types.h"
#include "Move.h"
#include "Pokemon.h"
#include "Trainer.h"
#include "Battle.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pokemon Battle GUI Test");
    window.setFramerateLimit(60);

    std::srand(static_cast<unsigned int>(std::time(0)));

    std::vector<Pokemon> allPokemon;

    Pokemon charmander("Charmander", Type::Fire, Type::None, 39, 52, 43, 65);
    charmander.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    charmander.addLearnMove(1, {"Growl", Type::Normal, 0, 100});
    charmander.addLearnMove(7, {"Ember", Type::Fire, 40, 100});
    charmander.addLearnMove(10, {"Metal Claw", Type::Normal, 50, 95});
    allPokemon.push_back(charmander);

    Pokemon squirtle("Squirtle", Type::Water, Type::None, 44, 48, 65, 43);
    squirtle.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    squirtle.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100});
    squirtle.addLearnMove(7, {"Bubble", Type::Water, 20, 100});
    squirtle.addLearnMove(10, {"Withdraw", Type::Water, 0, 100});
    allPokemon.push_back(squirtle);

    Pokemon bulbasaur("Bulbasaur", Type::Grass, Type::None, 45, 49, 49, 45);
    bulbasaur.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    bulbasaur.addLearnMove(1, {"Growl", Type::Normal, 0, 100});
    bulbasaur.addLearnMove(7, {"Leech Seed", Type::Grass, 0, 90});
    bulbasaur.addLearnMove(10, {"Vine Whip", Type::Grass, 45, 100});
    allPokemon.push_back(bulbasaur);
    
    Pokemon pidgey("Pidgey", Type::Normal, Type::None, 40, 45, 40, 56);
    pidgey.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    pidgey.addLearnMove(5, {"Sand Attack", Type::Normal, 0, 100});
    pidgey.addLearnMove(9, {"Gust", Type::Normal, 40, 100});
    allPokemon.push_back(pidgey);

    Pokemon pikachu("Pikachu", Type::Electric, Type::None, 35, 55, 40, 90);
    pikachu.addLearnMove(1, {"Thunder Shock", Type::Electric, 40, 100});
    pikachu.addLearnMove(1, {"Growl", Type::Normal, 0, 100});
    pikachu.addLearnMove(6, {"Tail Whip", Type::Normal, 0, 100});
    pikachu.addLearnMove(10, {"Quick Attack", Type::Normal, 40, 100});
    allPokemon.push_back(pikachu);

    Pokemon geodude("Geodude", Type::Normal, Type::None, 40, 80, 100, 20); 
    geodude.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    geodude.addLearnMove(1, {"Defense Curl", Type::Normal, 0, 100});
    geodude.addLearnMove(8, {"Mud Sport", Type::Normal, 0, 100});
    geodude.addLearnMove(10, {"Rock Throw", Type::Normal, 50, 90});
    allPokemon.push_back(geodude);

    Pokemon oddish("Oddish", Type::Grass, Type::None, 45, 50, 55, 30);
    oddish.addLearnMove(1, {"Absorb", Type::Grass, 20, 100});
    oddish.addLearnMove(7, {"Sweet Scent", Type::Normal, 0, 100});
    oddish.addLearnMove(9, {"Acid", Type::Normal, 40, 100});
    allPokemon.push_back(oddish);

    Pokemon abra("Abra", Type::Psychic, Type::None, 25, 20, 15, 90);
    abra.addLearnMove(1, {"Teleport", Type::Normal, 0, 100});
    abra.addLearnMove(10, {"Confusion", Type::Psychic, 50, 100});
    allPokemon.push_back(abra);

    Trainer player("Red", 3);
    Trainer rival("Blue", 3);

    std::random_device rd;
    std::mt19937 g(rd());

    while (window.isOpen()) {
        std::vector<Pokemon> pool = allPokemon;
        std::shuffle(pool.begin(), pool.end(), g);

        player.party.clear();
        rival.party.clear();

        // Assign random levels (8 to 10)
        std::uniform_int_distribution<> lvlDist(8, 10);
        pool[0].setLevel(lvlDist(g));
        pool[1].setLevel(lvlDist(g));
        pool[2].setLevel(lvlDist(g));
        pool[3].setLevel(lvlDist(g));

        player.addPokemon(pool[0]);
        player.addPokemon(pool[1]);

        rival.addPokemon(pool[2]);
        rival.addPokemon(pool[3]);

        player.healAll();
        rival.healAll();
        Battle battle(&player, &rival);
        battle.run(window);
    }

    return 0;
}
