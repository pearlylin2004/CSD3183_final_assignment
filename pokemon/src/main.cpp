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

#include "GameState.h"
#include "GreedyAgent.h"
#include <chrono>

int main() {
    std::cout << "=== POKEMON AI SIMULATOR ===\n";
    std::cout << "1. Play Game (GUI)\n";
    std::cout << "2. Run AI Benchmark (Greedy vs Greedy - 500 Matches)\n";
    std::cout << "Select mode (1 or 2): ";
    int mode = 1;
    std::cin >> mode;

    std::srand(static_cast<unsigned int>(std::time(0)));

    std::vector<Pokemon> allPokemon;

    Pokemon charmander("Charmander", Type::Fire, Type::None, 39, 52, 43, 65);
    charmander.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    charmander.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    charmander.addLearnMove(7, {"Ember", Type::Fire, 40, 100});
    charmander.addLearnMove(10, {"Metal Claw", Type::Normal, 50, 95});
    allPokemon.push_back(charmander);

    Pokemon squirtle("Squirtle", Type::Water, Type::None, 44, 48, 65, 43);
    squirtle.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    squirtle.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    squirtle.addLearnMove(7, {"Bubble", Type::Water, 20, 100});
    squirtle.addLearnMove(10, {"Withdraw", Type::Water, 0, 100, Stat::Defense, 1, true});
    allPokemon.push_back(squirtle);

    Pokemon bulbasaur("Bulbasaur", Type::Grass, Type::None, 45, 49, 49, 45);
    bulbasaur.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    bulbasaur.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    bulbasaur.addLearnMove(7, {"Leech Seed", Type::Grass, 0, 90});
    bulbasaur.addLearnMove(10, {"Vine Whip", Type::Grass, 45, 100});
    allPokemon.push_back(bulbasaur);
    
    Pokemon pidgey("Pidgey", Type::Normal, Type::None, 40, 45, 40, 56);
    pidgey.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    pidgey.addLearnMove(5, {"Sand Attack", Type::Normal, 0, 100, Stat::Accuracy, -1, false});
    pidgey.addLearnMove(9, {"Gust", Type::Normal, 40, 100});
    allPokemon.push_back(pidgey);

    Pokemon pikachu("Pikachu", Type::Electric, Type::None, 35, 55, 40, 90);
    pikachu.addLearnMove(1, {"Thunder Shock", Type::Electric, 40, 100});
    pikachu.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    pikachu.addLearnMove(6, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    pikachu.addLearnMove(10, {"Quick Attack", Type::Normal, 40, 100});
    allPokemon.push_back(pikachu);

    Pokemon geodude("Geodude", Type::Normal, Type::None, 40, 80, 100, 20); 
    geodude.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    geodude.addLearnMove(1, {"Defense Curl", Type::Normal, 0, 100, Stat::Defense, 1, true});
    geodude.addLearnMove(8, {"Mud Sport", Type::Normal, 0, 100, Stat::Attack, -1, false});
    geodude.addLearnMove(10, {"Rock Throw", Type::Normal, 50, 90});
    allPokemon.push_back(geodude);

    Pokemon oddish("Oddish", Type::Grass, Type::None, 45, 50, 55, 30);
    oddish.addLearnMove(1, {"Absorb", Type::Grass, 20, 100});
    oddish.addLearnMove(7, {"Sweet Scent", Type::Normal, 0, 100, Stat::Evasion, -1, false});
    oddish.addLearnMove(9, {"Acid", Type::Normal, 40, 100});
    allPokemon.push_back(oddish);

    Pokemon abra("Abra", Type::Psychic, Type::None, 25, 20, 15, 90);
    abra.addLearnMove(1, {"Teleport", Type::Normal, 0, 100});
    abra.addLearnMove(10, {"Confusion", Type::Psychic, 50, 100});
    allPokemon.push_back(abra);

    std::random_device rd;
    std::mt19937 g(rd());

    if (mode == 2) {
        int p1Wins = 0;
        int p2Wins = 0;
        int draws = 0;
        
        GreedyAgent agent1;
        GreedyAgent agent2;
        
        std::cout << "Running 500 benchmark matches (Greedy vs Greedy)...\n";
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 500; ++i) {
            std::vector<Pokemon> pool = allPokemon;
            std::shuffle(pool.begin(), pool.end(), g);
            
            Trainer player("Greedy P1", 3);
            Trainer rival("Greedy P2", 3);
            
            std::uniform_int_distribution<> lvlDist(8, 10);
            pool[0].setLevel(lvlDist(g));
            pool[1].setLevel(lvlDist(g));
            pool[2].setLevel(lvlDist(g));
            pool[3].setLevel(lvlDist(g));
            
            player.addPokemon(pool[0]);
            player.addPokemon(pool[1]);
            rival.addPokemon(pool[2]);
            rival.addPokemon(pool[3]);
            
            GameState state(player, rival);
            
            int turns = 0;
            int result = 0;
            while (result == 0 && turns < 100) {
                Action a1 = agent1.getAction(state, 1);
                Action a2 = agent2.getAction(state, 2);
                result = state.step(a1, a2);
                turns++;
            }
            
            if (result == 1) p1Wins++;
            else if (result == 2) p2Wins++;
            else draws++;
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
        
        std::cout << "\n=== BENCHMARK RESULTS ===\n";
        std::cout << "Total Matches: 500\n";
        std::cout << "Player 1 (Greedy) Wins: " << p1Wins << "\n";
        std::cout << "Player 2 (Greedy) Wins: " << p2Wins << "\n";
        std::cout << "Draws (Timeout): " << draws << "\n";
        std::cout << "Total Simulation Time: " << elapsed.count() << " ms\n";
        std::cout << "Average Time Per Match: " << (elapsed.count() / 500.0) << " ms\n";
        std::cout << "=========================\n";
        
        std::cout << "\nPress Enter to exit.\n";
        std::cin.ignore();
        std::cin.get();
        return 0;
    }

    // mode == 1 (Play Game)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pokemon Battle GUI Test");
    window.setFramerateLimit(60);

    Trainer player("Red", 3);
    Trainer rival("Blue", 3);

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
