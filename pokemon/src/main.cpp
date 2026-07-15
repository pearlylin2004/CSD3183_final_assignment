#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <fstream>

#include "Types.h"
#include "Move.h"
#include "Pokemon.h"
#include "Trainer.h"
#include "Battle.h"
#include "GameState.h"
#include "GreedyAgent.h"
#include "SMABAgent.h"
#include "MCTSAgent.h"

std::vector<Pokemon> initAllPokemon() {
    std::vector<Pokemon> allPokemon;

    Pokemon charmander("Charmander", Type::Fire, Type::None, 39, 52, 43, 65);
    charmander.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    charmander.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    charmander.addLearnMove(7, {"Ember", Type::Fire, 40, 100});
    charmander.addLearnMove(10, {"Metal Claw", Type::Normal, 50, 95});
    charmander.addEvolution(16, "Charmeleon", Type::Fire, Type::None, 58, 64, 58, 80);
    charmander.addLearnMove(25, {"Fire Fang", Type::Fire, 65, 95});
    charmander.addLearnMove(34, {"Flamethrower", Type::Fire, 90, 100});
    charmander.addEvolution(36, "Charizard", Type::Fire, Type::Flying, 78, 84, 78, 100);
    charmander.addLearnMove(36, {"Wing Attack", Type::Flying, 60, 100});
    charmander.addLearnMove(46, {"Fire Blast", Type::Fire, 110, 85});
    allPokemon.push_back(charmander);

    Pokemon squirtle("Squirtle", Type::Water, Type::None, 44, 48, 65, 43);
    squirtle.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    squirtle.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    squirtle.addLearnMove(7, {"Bubble", Type::Water, 20, 100});
    squirtle.addLearnMove(10, {"Withdraw", Type::Water, 0, 100, Stat::Defense, 1, true});
    squirtle.addLearnMove(13, {"Water Gun", Type::Water, 40, 100});
    squirtle.addEvolution(16, "Wartortle", Type::Water, Type::None, 59, 63, 80, 58);
    squirtle.addLearnMove(27, {"Bite", Type::Dark, 60, 100});
    squirtle.addLearnMove(34, {"Surf", Type::Water, 90, 100});
    squirtle.addEvolution(36, "Blastoise", Type::Water, Type::None, 79, 83, 100, 78);
    squirtle.addLearnMove(36, {"Flash Cannon", Type::Steel, 80, 100});
    squirtle.addLearnMove(47, {"Hydro Pump", Type::Water, 110, 80});
    allPokemon.push_back(squirtle);

    Pokemon bulbasaur("Bulbasaur", Type::Grass, Type::None, 45, 49, 49, 45);
    bulbasaur.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    bulbasaur.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    bulbasaur.addLearnMove(7, {"Leech Seed", Type::Grass, 0, 90});
    bulbasaur.addLearnMove(10, {"Vine Whip", Type::Grass, 45, 100});
    bulbasaur.addEvolution(16, "Ivysaur", Type::Grass, Type::Poison, 60, 62, 63, 60);
    bulbasaur.addLearnMove(20, {"Razor Leaf", Type::Grass, 55, 95});
    bulbasaur.addLearnMove(27, {"Take Down", Type::Normal, 90, 85});
    bulbasaur.addEvolution(32, "Venusaur", Type::Grass, Type::Poison, 80, 82, 83, 80);
    bulbasaur.addLearnMove(32, {"Petal Dance", Type::Grass, 120, 100});
    bulbasaur.addLearnMove(37, {"Seed Bomb", Type::Grass, 80, 100});
    bulbasaur.addLearnMove(46, {"Solar Beam", Type::Grass, 120, 100});
    allPokemon.push_back(bulbasaur);
    
    Pokemon pidgey("Pidgey", Type::Normal, Type::None, 40, 45, 40, 56);
    pidgey.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    pidgey.addLearnMove(5, {"Sand Attack", Type::Normal, 0, 100, Stat::Accuracy, -1, false});
    pidgey.addLearnMove(9, {"Gust", Type::Flying, 40, 100});
    pidgey.addLearnMove(13, {"Quick Attack", Type::Normal, 40, 100});
    pidgey.addEvolution(18, "Pidgeotto", Type::Normal, Type::Flying, 63, 60, 55, 71);
    pidgey.addLearnMove(25, {"Wing Attack", Type::Flying, 60, 100});
    pidgey.addEvolution(36, "Pidgeot", Type::Normal, Type::Flying, 83, 80, 75, 101);
    pidgey.addLearnMove(36, {"Hurricane", Type::Flying, 110, 70});
    pidgey.addLearnMove(43, {"Brave Bird", Type::Flying, 120, 100});
    allPokemon.push_back(pidgey);

    Pokemon pikachu("Pikachu", Type::Electric, Type::None, 35, 55, 40, 90);
    pikachu.addLearnMove(1, {"Thunder Shock", Type::Electric, 40, 100});
    pikachu.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    pikachu.addLearnMove(6, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    pikachu.addLearnMove(10, {"Quick Attack", Type::Normal, 40, 100});
    pikachu.addLearnMove(21, {"Spark", Type::Electric, 65, 100});
    pikachu.addLearnMove(29, {"Thunderbolt", Type::Electric, 90, 100});
    pikachu.addLearnMove(42, {"Thunder", Type::Electric, 110, 70});
    allPokemon.push_back(pikachu);

    Pokemon geodude("Geodude", Type::Rock, Type::Ground, 40, 80, 100, 20); 
    geodude.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    geodude.addLearnMove(1, {"Defense Curl", Type::Normal, 0, 100, Stat::Defense, 1, true});
    geodude.addLearnMove(8, {"Mud Sport", Type::Ground, 0, 100, Stat::Attack, -1, false});
    geodude.addLearnMove(10, {"Rock Throw", Type::Rock, 50, 90});
    geodude.addLearnMove(22, {"Rock Slide", Type::Rock, 75, 90});
    geodude.addEvolution(25, "Graveler", Type::Rock, Type::Ground, 55, 95, 115, 35);
    geodude.addLearnMove(30, {"Earthquake", Type::Ground, 100, 100});
    geodude.addEvolution(40, "Golem", Type::Rock, Type::Ground, 80, 120, 130, 45);
    geodude.addLearnMove(40, {"Heavy Slam", Type::Steel, 100, 100});
    geodude.addLearnMove(40, {"Stone Edge", Type::Rock, 100, 80});
    allPokemon.push_back(geodude);

    Pokemon oddish("Oddish", Type::Grass, Type::None, 45, 50, 55, 30);
    oddish.addLearnMove(1, {"Absorb", Type::Grass, 20, 100});
    oddish.addLearnMove(7, {"Sweet Scent", Type::Normal, 0, 100, Stat::Evasion, -1, false});
    oddish.addLearnMove(9, {"Acid", Type::Normal, 40, 100});
    oddish.addEvolution(21, "Gloom", Type::Grass, Type::Poison, 60, 65, 70, 40);
    oddish.addLearnMove(21, {"Mega Drain", Type::Grass, 40, 100});
    oddish.addLearnMove(35, {"Giga Drain", Type::Grass, 75, 100});
    oddish.addEvolution(36, "Vileplume", Type::Grass, Type::Poison, 75, 80, 85, 50);
    oddish.addLearnMove(45, {"Petal Blizzard", Type::Grass, 90, 100});
    allPokemon.push_back(oddish);

    Pokemon abra("Abra", Type::Psychic, Type::None, 25, 20, 15, 90);
    abra.addLearnMove(1, {"Teleport", Type::Normal, 0, 100});
    abra.addLearnMove(10, {"Confusion", Type::Psychic, 50, 100});
    abra.addEvolution(16, "Kadabra", Type::Psychic, Type::None, 40, 35, 30, 105);
    abra.addLearnMove(21, {"Psybeam", Type::Psychic, 65, 100});
    abra.addEvolution(36, "Alakazam", Type::Psychic, Type::None, 55, 50, 45, 120);
    abra.addLearnMove(36, {"Psychic", Type::Psychic, 90, 100});
    allPokemon.push_back(abra);

    Pokemon caterpie("Caterpie", Type::Bug, Type::None, 45, 30, 35, 45);
    caterpie.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    caterpie.addLearnMove(1, {"String Shot", Type::Bug, 0, 95, Stat::Speed, -1, false});
    caterpie.addEvolution(7, "Metapod", Type::Bug, Type::None, 50, 20, 55, 30);
    caterpie.addEvolution(10, "Butterfree", Type::Bug, Type::Flying, 60, 45, 50, 70);
    caterpie.addLearnMove(10, {"Confusion", Type::Psychic, 50, 100});
    caterpie.addLearnMove(15, {"Bug Bite", Type::Bug, 60, 100});
    caterpie.addLearnMove(24, {"Psybeam", Type::Psychic, 65, 100});
    caterpie.addLearnMove(36, {"Bug Buzz", Type::Bug, 90, 100});
    allPokemon.push_back(caterpie);

    Pokemon weedle("Weedle", Type::Bug, Type::Poison, 40, 35, 30, 50);
    weedle.addLearnMove(1, {"Poison Sting", Type::Poison, 15, 100});
    weedle.addLearnMove(1, {"String Shot", Type::Bug, 0, 95, Stat::Speed, -1, false});
    weedle.addEvolution(7, "Kakuna", Type::Bug, Type::Poison, 45, 25, 50, 35);
    weedle.addEvolution(10, "Beedrill", Type::Bug, Type::Poison, 65, 90, 40, 75);
    weedle.addLearnMove(10, {"Twineedle", Type::Bug, 50, 100});
    weedle.addLearnMove(15, {"Poison Jab", Type::Poison, 80, 100});
    weedle.addLearnMove(24, {"Pin Missile", Type::Bug, 75, 95});
    weedle.addLearnMove(36, {"Sludge Bomb", Type::Poison, 90, 100});
    allPokemon.push_back(weedle);

    Pokemon rattata("Rattata", Type::Normal, Type::None, 30, 56, 35, 72);
    rattata.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    rattata.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    rattata.addLearnMove(7, {"Quick Attack", Type::Normal, 40, 100});
    rattata.addLearnMove(14, {"Hyper Fang", Type::Normal, 80, 90});
    rattata.addEvolution(20, "Raticate", Type::Normal, Type::None, 55, 81, 60, 97);
    rattata.addLearnMove(20, {"Crunch", Type::Dark, 80, 100});
    rattata.addLearnMove(30, {"Double-Edge", Type::Normal, 120, 100});
    rattata.addLearnMove(40, {"Super Fang", Type::Normal, 100, 90});
    allPokemon.push_back(rattata);

    Pokemon spearow("Spearow", Type::Normal, Type::Flying, 40, 60, 30, 70);
    spearow.addLearnMove(1, {"Peck", Type::Flying, 35, 100});
    spearow.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    spearow.addLearnMove(9, {"Pursuit", Type::Dark, 40, 100});
    spearow.addLearnMove(15, {"Aerial Ace", Type::Flying, 60, 100});
    spearow.addEvolution(20, "Fearow", Type::Normal, Type::Flying, 65, 90, 65, 100);
    spearow.addLearnMove(20, {"Drill Run", Type::Ground, 80, 95});
    spearow.addLearnMove(30, {"Drill Peck", Type::Flying, 80, 100});
    spearow.addLearnMove(45, {"Sky Attack", Type::Flying, 140, 90});
    allPokemon.push_back(spearow);

    Pokemon ekans("Ekans", Type::Poison, Type::None, 35, 60, 44, 55);
    ekans.addLearnMove(1, {"Wrap", Type::Normal, 15, 90});
    ekans.addLearnMove(1, {"Leer", Type::Normal, 0, 100, Stat::Defense, -1, false});
    ekans.addLearnMove(9, {"Poison Sting", Type::Poison, 15, 100});
    ekans.addLearnMove(14, {"Bite", Type::Dark, 60, 100});
    ekans.addEvolution(22, "Arbok", Type::Poison, Type::None, 60, 95, 69, 80);
    ekans.addLearnMove(22, {"Crunch", Type::Dark, 80, 100});
    ekans.addLearnMove(30, {"Sludge Bomb", Type::Poison, 90, 100});
    ekans.addLearnMove(45, {"Gunk Shot", Type::Poison, 120, 80});
    allPokemon.push_back(ekans);

    Pokemon sandshrew("Sandshrew", Type::Ground, Type::None, 50, 75, 85, 40);
    sandshrew.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    sandshrew.addLearnMove(6, {"Defense Curl", Type::Normal, 0, 100, Stat::Defense, 1, true});
    sandshrew.addLearnMove(15, {"Magnitude", Type::Ground, 70, 100});
    sandshrew.addEvolution(22, "Sandslash", Type::Ground, Type::None, 75, 100, 110, 65);
    sandshrew.addLearnMove(22, {"Crush Claw", Type::Normal, 75, 95, Stat::Defense, -1, false});
    sandshrew.addLearnMove(30, {"Earthquake", Type::Ground, 100, 100});
    sandshrew.addLearnMove(45, {"Fissure", Type::Ground, 150, 30});
    allPokemon.push_back(sandshrew);

    Pokemon nidoranf("Nidoran-f", Type::Poison, Type::None, 55, 47, 52, 41);
    nidoranf.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    nidoranf.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    nidoranf.addLearnMove(12, {"Double Kick", Type::Fighting, 60, 100});
    nidoranf.addEvolution(16, "Nidorina", Type::Poison, Type::None, 70, 62, 67, 56);
    nidoranf.addLearnMove(30, {"Poison Fang", Type::Poison, 50, 100});
    nidoranf.addEvolution(36, "Nidoqueen", Type::Poison, Type::Ground, 90, 92, 87, 76);
    nidoranf.addLearnMove(45, {"Earth Power", Type::Ground, 90, 100});
    allPokemon.push_back(nidoranf);

    Pokemon nidoranm("Nidoran-m", Type::Poison, Type::None, 46, 57, 40, 50);
    nidoranm.addLearnMove(1, {"Leer", Type::Normal, 0, 100, Stat::Defense, -1, false});
    nidoranm.addLearnMove(1, {"Peck", Type::Flying, 35, 100});
    nidoranm.addLearnMove(12, {"Double Kick", Type::Fighting, 60, 100});
    nidoranm.addEvolution(16, "Nidorino", Type::Poison, Type::None, 61, 72, 57, 65);
    nidoranm.addLearnMove(30, {"Poison Jab", Type::Poison, 80, 100});
    nidoranm.addEvolution(36, "Nidoking", Type::Poison, Type::Ground, 81, 102, 77, 85);
    nidoranm.addLearnMove(45, {"Megahorn", Type::Bug, 120, 85});
    allPokemon.push_back(nidoranm);

    Pokemon clefairy("Clefairy", Type::Fairy, Type::None, 70, 45, 48, 35);
    clefairy.addLearnMove(1, {"Pound", Type::Normal, 40, 100});
    clefairy.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    clefairy.addLearnMove(15, {"Magical Leaf", Type::Grass, 60, 100});
    clefairy.addLearnMove(30, {"Moonblast", Type::Fairy, 95, 100});
    clefairy.addEvolution(36, "Clefable", Type::Fairy, Type::None, 95, 70, 73, 60);
    clefairy.addLearnMove(45, {"Meteor Mash", Type::Steel, 90, 90});
    allPokemon.push_back(clefairy);

    Pokemon vulpix("Vulpix", Type::Fire, Type::None, 38, 41, 40, 65);
    vulpix.addLearnMove(1, {"Ember", Type::Fire, 40, 100});
    vulpix.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    vulpix.addLearnMove(15, {"Will-O-Wisp", Type::Fire, 0, 85, Stat::Attack, -1, false});
    vulpix.addLearnMove(30, {"Flamethrower", Type::Fire, 90, 100});
    vulpix.addEvolution(36, "Ninetales", Type::Fire, Type::None, 73, 76, 75, 100);
    vulpix.addLearnMove(45, {"Fire Blast", Type::Fire, 110, 85});
    allPokemon.push_back(vulpix);

    Pokemon jigglypuff("Jigglypuff", Type::Normal, Type::Fairy, 115, 45, 20, 20);
    jigglypuff.addLearnMove(1, {"Sing", Type::Normal, 0, 55});
    jigglypuff.addLearnMove(9, {"Pound", Type::Normal, 40, 100});
    jigglypuff.addLearnMove(20, {"Body Slam", Type::Normal, 85, 100});
    jigglypuff.addLearnMove(30, {"Hyper Voice", Type::Normal, 90, 100});
    jigglypuff.addEvolution(36, "Wigglytuff", Type::Normal, Type::Fairy, 140, 70, 45, 45);
    jigglypuff.addLearnMove(45, {"Play Rough", Type::Fairy, 90, 90});
    allPokemon.push_back(jigglypuff);

    Pokemon zubat("Zubat", Type::Poison, Type::Flying, 40, 45, 35, 55);
    zubat.addLearnMove(1, {"Leech Life", Type::Bug, 20, 100});
    zubat.addLearnMove(6, {"Astonish", Type::Ghost, 30, 100});
    zubat.addLearnMove(12, {"Bite", Type::Dark, 60, 100});
    zubat.addLearnMove(21, {"Wing Attack", Type::Flying, 60, 100});
    zubat.addEvolution(22, "Golbat", Type::Poison, Type::Flying, 75, 80, 70, 90);
    zubat.addLearnMove(35, {"Cross Poison", Type::Poison, 70, 100});
    zubat.addEvolution(42, "Crobat", Type::Poison, Type::Flying, 85, 90, 80, 130);
    zubat.addLearnMove(45, {"Acrobatics", Type::Flying, 110, 100});
    allPokemon.push_back(zubat);

    return allPokemon;
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::srand(static_cast<unsigned int>(std::time(0)));

    std::vector<Pokemon> allPokemon = initAllPokemon();
    std::random_device rd;
    std::mt19937 g(rd());

    while (true) {
        std::cout << "\n=== POKEMON AI SIMULATOR ===\n";
        std::cout << "1. Play Game (GUI)\n";
        std::cout << "2. Run AI Benchmark (Greedy vs Greedy)\n";
        std::cout << "3. Run AI Benchmark (SMAB vs Greedy)\n";
        std::cout << "4. Run AI Benchmark (MCTS vs Greedy)\n";
        std::cout << "5. Run All Benchmarks\n";
        std::cout << "6. Run AI Benchmark with GUI (Greedy vs Greedy)\n";
        std::cout << "7. Run AI Benchmark with GUI (SMAB vs Greedy)\n";
        std::cout << "8. Run AI Benchmark with GUI (MCTS vs Greedy)\n";
        std::cout << "9. Run All Benchmarks with GUI\n";
        std::cout << "10. Exit\n";
        std::cout << "Select mode (1-10): ";
        
        int mode;
        if (!(std::cin >> mode)) break;
        
        if (mode == 10) break;
        
        int guiAgentChoice = 1;
        if (mode == 1) {
            std::cout << "Select opponent AI for GUI Battle:\n";
            std::cout << "1. Random (Basic)\n";
            std::cout << "2. Greedy Agent\n";
            std::cout << "3. SMAB Agent\n";
            std::cout << "4. MCTS Agent\n";
            std::cout << "Choice: ";
            if (!(std::cin >> guiAgentChoice)) break;
        }
        
        int minLevel = 8, maxLevel = 10;
        while (true) {
            std::cout << "Enter minimum Pokemon level (1-100): ";
            if (!(std::cin >> minLevel)) break;
            std::cout << "Enter maximum Pokemon level (1-100): ";
            if (!(std::cin >> maxLevel)) break;
            
            if (minLevel < 1) minLevel = 1;
            if (maxLevel > 100) maxLevel = 100;
            
            if (maxLevel < minLevel) {
                std::cout << "Error: Maximum level cannot be lower than minimum level! Please try again.\n";
            } else {
                break;
            }
        }
        if (!std::cin) break;
        
        if (mode == 1) {
            sf::RenderWindow window(sf::VideoMode({800, 600}), "Pokemon Battle GUI Test");
            window.setFramerateLimit(60);

            Trainer player("Red", 6);
            Trainer rival("Blue", 6);

            while (window.isOpen()) {
                std::vector<Pokemon> pool = allPokemon;
                std::shuffle(pool.begin(), pool.end(), g);

                player.party.clear();
                rival.party.clear();

                std::uniform_int_distribution<> lvlDist(minLevel, maxLevel);
                for (int p = 0; p < 12; ++p) {
                    pool[p].setLevel(lvlDist(g));
                }

                player.addPokemon(pool[0]); player.addPokemon(pool[1]); player.addPokemon(pool[2]); player.addPokemon(pool[3]); player.addPokemon(pool[4]); player.addPokemon(pool[5]);
                rival.addPokemon(pool[6]); rival.addPokemon(pool[7]); rival.addPokemon(pool[8]); rival.addPokemon(pool[9]); rival.addPokemon(pool[10]); rival.addPokemon(pool[11]);

                player.healAll();
                rival.healAll();
                
                GreedyAgent greedyAgent;
                SMABAgent smabAgent;
                MCTSAgent mctsAgent;
                Agent* selectedAgent = nullptr;
                if (guiAgentChoice == 2) selectedAgent = &greedyAgent;
                else if (guiAgentChoice == 3) selectedAgent = &smabAgent;
                else if (guiAgentChoice == 4) selectedAgent = &mctsAgent;
                
                Battle battle(&player, &rival, selectedAgent);
                battle.run(window);
            }
        } else if ((mode >= 2 && mode <= 5) || (mode >= 6 && mode <= 9)) {
            bool useGUI = (mode >= 6 && mode <= 9);
            
            int totalMatches = useGUI ? 5 : 20;
            std::cout << "Enter number of matches to simulate: ";
            if (!(std::cin >> totalMatches)) break;
            
            sf::RenderWindow* windowPtr = nullptr;
            sf::Font font;
            sf::Texture playerTexture;
            sf::Texture enemyTexture;
            std::optional<sf::Sprite> playerSprite;
            std::optional<sf::Sprite> enemySprite;
            std::string lastP1Name = "";
            std::string lastP2Name = "";
            
            if (useGUI) {
                windowPtr = new sf::RenderWindow(sf::VideoMode({800, 600}), "Pokemon AI Sped-up GUI Benchmark");
                windowPtr->setFramerateLimit(60);
                if (!font.openFromFile("assets/arial.ttf"))
                    std::cout << "Failed to load font!\n";
            }

            int baseMode = (mode >= 6) ? (mode - 4) : mode;
            int startMode = (baseMode == 5) ? 2 : baseMode;
            int endMode = (baseMode == 5) ? 4 : baseMode;

            for (int currentMode = startMode; currentMode <= endMode; ++currentMode) {
                if (useGUI && !windowPtr->isOpen()) break;
                
                int p1Wins = 0, p2Wins = 0, draws = 0;
                
                std::ofstream logFile("battle_log.txt");
                
                Agent* agent1 = nullptr;
                std::string p1Name = "";
                if (currentMode == 2) { agent1 = new GreedyAgent(); p1Name = "Greedy"; }
                else if (currentMode == 3) { agent1 = new SMABAgent(2); p1Name = "SMAB"; }
                else if (currentMode == 4) { agent1 = new MCTSAgent(100); p1Name = "MCTS"; }
                
                GreedyAgent agent2; 
                
                if (!useGUI)
                    std::cout << "\nRunning " << totalMatches << " benchmark matches (" << p1Name << " vs Greedy)..." << std::flush;

                auto startTime = std::chrono::high_resolution_clock::now();
                
                for (int i = 0; i < totalMatches; ++i) {
                    if (useGUI && !windowPtr->isOpen()) break;
                    
                    std::vector<Pokemon> pool = allPokemon;
                    std::shuffle(pool.begin(), pool.end(), g);
                    
                    Trainer player(p1Name, 6);
                    Trainer rival("Greedy", 6);
                    
                    std::uniform_int_distribution<> lvlDist(minLevel, maxLevel);
                    for (int p = 0; p < 12; ++p) pool[p].setLevel(lvlDist(g));
                    
                    player.addPokemon(pool[0]); player.addPokemon(pool[1]); player.addPokemon(pool[2]); player.addPokemon(pool[3]); player.addPokemon(pool[4]); player.addPokemon(pool[5]);
                    rival.addPokemon(pool[6]); rival.addPokemon(pool[7]); rival.addPokemon(pool[8]); rival.addPokemon(pool[9]); rival.addPokemon(pool[10]); rival.addPokemon(pool[11]);
                    
                    GameState state(player, rival);
                    int turns = 0, result = 0;
                    std::string actionLog = "Battle Start!";
                    
                    while (result == 0 && turns < 1000) {
                        if (useGUI) {
                            if (!windowPtr->isOpen()) break;
                            while (const std::optional<sf::Event> event = windowPtr->pollEvent())
                                if (event->is<sf::Event::Closed>()) 
                                    windowPtr->close();
                        }
                        
                        Action a1 = agent1->getAction(state, 1);
                        Action a2 = agent2.getAction(state, 2);
                        
                        Pokemon* oldP1 = state.player1.getActivePokemon();
                        Pokemon* oldP2 = state.player2.getActivePokemon();
                        actionLog = "";
                        
                        std::string p1MoveStr = "";
                        if (a1.type == ActionType::MOVE && oldP1) p1MoveStr = oldP1->name + " used " + oldP1->moves[a1.index].name + "!";
                        else if (a1.type == ActionType::SWITCH) p1MoveStr = p1Name + " switched to " + state.player1.party[a1.index].name + "!";
                        else if (a1.type == ActionType::POTION) p1MoveStr = p1Name + " used a Potion!";
                        
                        std::string p2MoveStr = "";
                        if (a2.type == ActionType::MOVE && oldP2) p2MoveStr = oldP2->name + " used " + oldP2->moves[a2.index].name + "!";
                        else if (a2.type == ActionType::SWITCH) p2MoveStr = "Greedy switched to " + state.player2.party[a2.index].name + "!";
                        else if (a2.type == ActionType::POTION) p2MoveStr = "Greedy used a Potion!";
                        
                        actionLog = p1MoveStr + "\n" + p2MoveStr;
                        if (actionLog.empty()) actionLog = "Waiting for next turn...";
                        
                        if (logFile.is_open()) {
                            logFile << "Turn " << turns << ":\n";
                            logFile << "  " << p1Name << ": " << p1MoveStr << "\n";
                            logFile << "  Greedy: " << p2MoveStr << "\n";
                        }
                        
                        result = state.step(a1, a2);
                        turns++;
                        
                        if (logFile.is_open()) {
                            Pokemon* newP1 = state.player1.getActivePokemon();
                            Pokemon* newP2 = state.player2.getActivePokemon();
                            logFile << "  End of Turn State:\n";
                            if (newP1) logFile << "    " << p1Name << "'s " << newP1->name << " HP: " << newP1->current_hp << "/" << newP1->max_hp << "\n";
                            if (newP2) logFile << "    Greedy's " << newP2->name << " HP: " << newP2->current_hp << "/" << newP2->max_hp << "\n";
                            logFile << "----------------------------------------\n";
                        }
                        
                        if (useGUI) {
                            windowPtr->clear(sf::Color(240, 240, 240));
                            Pokemon* p1Mon = state.player1.getActivePokemon();
                            Pokemon* p2Mon = state.player2.getActivePokemon();
                            
                            if (p2Mon) {
                                std::string eName = p2Mon->name;
                                std::transform(eName.begin(), eName.end(), eName.begin(), ::tolower);
                                if (eName != lastP2Name) {
                                    if (enemyTexture.loadFromFile("assets/front/" + eName + ".png")) {
                                        enemySprite = sf::Sprite(enemyTexture);
                                        enemySprite->setScale(sf::Vector2f(2.5f, 2.5f));
                                        enemySprite->setPosition(sf::Vector2f(500.f, 30.f));
                                    }
                                    lastP2Name = eName;
                                }
                                if (lastP2Name != "" && enemySprite) windowPtr->draw(*enemySprite);
                                
                                sf::Text eText(font, p2Mon->name + "  Lvl " + std::to_string(p2Mon->level), 24);
                                eText.setFillColor(sf::Color::Black); eText.setPosition(sf::Vector2f(50.f, 50.f)); windowPtr->draw(eText);
                                sf::RectangleShape eBarBg(sf::Vector2f(200.f, 15.f)); eBarBg.setFillColor(sf::Color::Red); eBarBg.setPosition(sf::Vector2f(50.f, 90.f));
                                float eRatio = std::max(0.0f, (float)p2Mon->current_hp / p2Mon->max_hp);
                                sf::RectangleShape eBar(sf::Vector2f(200.f * eRatio, 15.f)); eBar.setFillColor(sf::Color::Green); eBar.setPosition(sf::Vector2f(50.f, 90.f));
                                windowPtr->draw(eBarBg); windowPtr->draw(eBar);
                            }
                            
                            if (p1Mon) {
                                std::string pName = p1Mon->name;
                                std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
                                if (pName != lastP1Name) {
                                    if (playerTexture.loadFromFile("assets/back/" + pName + ".png")) {
                                        playerSprite = sf::Sprite(playerTexture);
                                        playerSprite->setScale(sf::Vector2f(3.f, 3.f));
                                        playerSprite->setPosition(sf::Vector2f(100.f, 150.f));
                                    }
                                    lastP1Name = pName;
                                }
                                if (lastP1Name != "" && playerSprite) windowPtr->draw(*playerSprite);
                                
                                sf::Text pText(font, p1Mon->name + "  Lvl " + std::to_string(p1Mon->level) + "\nHP: " + std::to_string(p1Mon->current_hp) + "/" + std::to_string(p1Mon->max_hp), 24);
                                pText.setFillColor(sf::Color::Black); pText.setPosition(sf::Vector2f(500.f, 280.f)); windowPtr->draw(pText);
                                sf::RectangleShape pBarBg(sf::Vector2f(200.f, 15.f)); pBarBg.setFillColor(sf::Color::Red); pBarBg.setPosition(sf::Vector2f(500.f, 350.f));
                                float pRatio = std::max(0.0f, (float)p1Mon->current_hp / p1Mon->max_hp);
                                sf::RectangleShape pBar(sf::Vector2f(200.f * pRatio, 15.f)); pBar.setFillColor(sf::Color::Green); pBar.setPosition(sf::Vector2f(500.f, 350.f));
                                windowPtr->draw(pBarBg); windowPtr->draw(pBar);
                            }
                            
                            sf::RectangleShape bottomPanel;
                            bottomPanel.setSize(sf::Vector2f(780.f, 180.f)); bottomPanel.setPosition(sf::Vector2f(10.f, 400.f));
                            bottomPanel.setFillColor(sf::Color::White); bottomPanel.setOutlineColor(sf::Color::Black); bottomPanel.setOutlineThickness(5.f);
                            windowPtr->draw(bottomPanel);
                            
                            sf::Text actionText(font, actionLog, 24); actionText.setFillColor(sf::Color::Black); actionText.setPosition(sf::Vector2f(30.f, 420.f));
                            windowPtr->draw(actionText);
                            
                            sf::Text infoText(font, "Match " + std::to_string(i + 1) + "/" + std::to_string(totalMatches) + " | Turn: " + std::to_string(turns) + " | " + p1Name + " vs Greedy", 20);
                            infoText.setFillColor(sf::Color::Black); infoText.setPosition(sf::Vector2f(10.f, 10.f));
                            windowPtr->draw(infoText);
                            
                            windowPtr->display();
                            sf::sleep(sf::milliseconds(50));
                        }
                    }
                    
                    if (result == 1) p1Wins++; else if (result == 2) p2Wins++; else draws++;
                    
                    if (!useGUI) {
                        const char spinner[] = {'|', '/', '-', '\\'};
                        std::cout << "\rRunning " << totalMatches << " benchmark matches (" << p1Name << " vs Greedy)... " << spinner[i % 4] << std::flush;
                    }
                }
                
                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
                
                if (useGUI)
                    std::cout << "\n=== SPED-UP GUI BENCHMARK RESULTS (" << p1Name << " vs Greedy) ===\n";
                else {
                    std::cout << "\rRunning " << totalMatches << " benchmark matches (" << p1Name << " vs Greedy)... Done!\n";
                    std::cout << "=== BENCHMARK RESULTS (" << p1Name << " vs Greedy) ===\n";
                }
                std::cout << "Total Matches: " << totalMatches << "\n";
                std::cout << p1Name << " Wins: " << p1Wins << " (" << (static_cast<float>(p1Wins) / totalMatches * 100.0f) << "%)\n";
                std::cout << "Greedy Wins: " << p2Wins << " (" << (static_cast<float>(p2Wins) / totalMatches * 100.0f) << "%)\n";
                std::cout << "Draws: " << draws << "\n";
                if (!useGUI) {
                    std::cout << "Total Simulation Time: " << elapsed.count() << " ms\n";
                    std::cout << "Average Time Per Match: " << (elapsed.count() / totalMatches) << " ms\n";
                }
                std::cout << "=========================================\n";
                
                delete agent1;
            }
            if (windowPtr) {
                if (windowPtr->isOpen()) windowPtr->close();
                delete windowPtr;
            }
        }
    }

    return 0;
}
