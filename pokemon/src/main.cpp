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

#include "Types.h"
#include "Move.h"
#include "Pokemon.h"
#include "Trainer.h"
#include "Battle.h"
#include "GameState.h"
#include "GreedyAgent.h"
#include "SMABAgent.h"
#include "MCTSAgent.h"

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

    Pokemon caterpie("Caterpie", Type::Bug, Type::None, 45, 30, 35, 45);
    caterpie.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    caterpie.addLearnMove(1, {"String Shot", Type::Bug, 0, 95, Stat::Speed, -1, false});
    allPokemon.push_back(caterpie);

    Pokemon weedle("Weedle", Type::Bug, Type::Poison, 40, 35, 30, 50);
    weedle.addLearnMove(1, {"Poison Sting", Type::Poison, 15, 100});
    weedle.addLearnMove(1, {"String Shot", Type::Bug, 0, 95, Stat::Speed, -1, false});
    allPokemon.push_back(weedle);

    Pokemon rattata("Rattata", Type::Normal, Type::None, 30, 56, 35, 72);
    rattata.addLearnMove(1, {"Tackle", Type::Normal, 40, 100});
    rattata.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    rattata.addLearnMove(7, {"Quick Attack", Type::Normal, 40, 100});
    allPokemon.push_back(rattata);

    Pokemon spearow("Spearow", Type::Normal, Type::Flying, 40, 60, 30, 70);
    spearow.addLearnMove(1, {"Peck", Type::Flying, 35, 100});
    spearow.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    spearow.addLearnMove(9, {"Pursuit", Type::Dark, 40, 100});
    allPokemon.push_back(spearow);

    Pokemon ekans("Ekans", Type::Poison, Type::None, 35, 60, 44, 55);
    ekans.addLearnMove(1, {"Wrap", Type::Normal, 15, 90});
    ekans.addLearnMove(1, {"Leer", Type::Normal, 0, 100, Stat::Defense, -1, false});
    ekans.addLearnMove(9, {"Poison Sting", Type::Poison, 15, 100});
    allPokemon.push_back(ekans);

    Pokemon sandshrew("Sandshrew", Type::Ground, Type::None, 50, 75, 85, 40);
    sandshrew.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    sandshrew.addLearnMove(6, {"Defense Curl", Type::Normal, 0, 100, Stat::Defense, 1, true});
    allPokemon.push_back(sandshrew);

    Pokemon nidoranf("Nidoran-f", Type::Poison, Type::None, 55, 47, 52, 41);
    nidoranf.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    nidoranf.addLearnMove(1, {"Scratch", Type::Normal, 40, 100});
    allPokemon.push_back(nidoranf);

    Pokemon nidoranm("Nidoran-m", Type::Poison, Type::None, 46, 57, 40, 50);
    nidoranm.addLearnMove(1, {"Leer", Type::Normal, 0, 100, Stat::Defense, -1, false});
    nidoranm.addLearnMove(1, {"Peck", Type::Flying, 35, 100});
    allPokemon.push_back(nidoranm);

    Pokemon clefairy("Clefairy", Type::Fairy, Type::None, 70, 45, 48, 35);
    clefairy.addLearnMove(1, {"Pound", Type::Normal, 40, 100});
    clefairy.addLearnMove(1, {"Growl", Type::Normal, 0, 100, Stat::Attack, -1, false});
    allPokemon.push_back(clefairy);

    Pokemon vulpix("Vulpix", Type::Fire, Type::None, 38, 41, 40, 65);
    vulpix.addLearnMove(1, {"Ember", Type::Fire, 40, 100});
    vulpix.addLearnMove(1, {"Tail Whip", Type::Normal, 0, 100, Stat::Defense, -1, false});
    allPokemon.push_back(vulpix);

    Pokemon jigglypuff("Jigglypuff", Type::Normal, Type::Fairy, 115, 45, 20, 20);
    jigglypuff.addLearnMove(1, {"Sing", Type::Normal, 0, 55});
    jigglypuff.addLearnMove(9, {"Pound", Type::Normal, 40, 100});
    allPokemon.push_back(jigglypuff);

    Pokemon zubat("Zubat", Type::Poison, Type::Flying, 40, 45, 35, 55);
    zubat.addLearnMove(1, {"Leech Life", Type::Bug, 20, 100});
    zubat.addLearnMove(6, {"Astonish", Type::Ghost, 30, 100});
    allPokemon.push_back(zubat);

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
        
        int mode = 0;
        if (!(std::cin >> mode)) {
            break;
        }

        if (mode == 10) {
            break;
        }
        
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

                std::uniform_int_distribution<> lvlDist(8, 10);
                for (int p = 0; p < 12; ++p) {
                    pool[p].setLevel(lvlDist(g));
                }

                player.addPokemon(pool[0]); player.addPokemon(pool[1]); player.addPokemon(pool[2]); player.addPokemon(pool[3]); player.addPokemon(pool[4]); player.addPokemon(pool[5]);
                rival.addPokemon(pool[6]); rival.addPokemon(pool[7]); rival.addPokemon(pool[8]); rival.addPokemon(pool[9]); rival.addPokemon(pool[10]); rival.addPokemon(pool[11]);

                player.healAll();
                rival.healAll();
                Battle battle(&player, &rival);
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
                    
                    std::uniform_int_distribution<> lvlDist(8, 10);
                    for (int p = 0; p < 12; ++p) pool[p].setLevel(lvlDist(g));
                    
                    player.addPokemon(pool[0]); player.addPokemon(pool[1]); player.addPokemon(pool[2]); player.addPokemon(pool[3]); player.addPokemon(pool[4]); player.addPokemon(pool[5]);
                    rival.addPokemon(pool[6]); rival.addPokemon(pool[7]); rival.addPokemon(pool[8]); rival.addPokemon(pool[9]); rival.addPokemon(pool[10]); rival.addPokemon(pool[11]);
                    
                    GameState state(player, rival);
                    int turns = 0, result = 0;
                    std::string actionLog = "Battle Start!";
                    
                    while (result == 0 && turns < 100) {
                        if (useGUI) {
                            if (!windowPtr->isOpen()) break;
                            while (const std::optional<sf::Event> event = windowPtr->pollEvent())
                                if (event->is<sf::Event::Closed>()) 
                                    windowPtr->close();
                        }
                        
                        Action a1 = agent1->getAction(state, 1);
                        Action a2 = agent2.getAction(state, 2);
                        
                        if (useGUI) {
                            Pokemon* oldP1 = state.player1.getActivePokemon();
                            Pokemon* oldP2 = state.player2.getActivePokemon();
                            actionLog = "";
                            if (a1.type == ActionType::MOVE && oldP1) actionLog += oldP1->name + " used " + oldP1->moves[a1.index].name + "!\n";
                            else if (a1.type == ActionType::SWITCH) actionLog += p1Name + " switched to " + state.player1.party[a1.index].name + "!\n";
                            else if (a1.type == ActionType::POTION) actionLog += p1Name + " used a Potion!\n";
                            
                            if (a2.type == ActionType::MOVE && oldP2) actionLog += oldP2->name + " used " + oldP2->moves[a2.index].name + "!\n";
                            else if (a2.type == ActionType::SWITCH) actionLog += "Greedy switched to " + state.player2.party[a2.index].name + "!\n";
                            else if (a2.type == ActionType::POTION) actionLog += "Greedy used a Potion!\n";
                            
                            if (actionLog.empty()) actionLog = "Waiting for next turn...";
                        }
                        
                        result = state.step(a1, a2);
                        turns++;
                        
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
