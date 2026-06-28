#include "Battle.h"
#include <iostream>
#include <cstdlib>

Battle::Battle(Trainer* p, Trainer* e) : player(p), enemy(e), currentState(BattleState::Action_Text), selectedMenuIndex(0), isFinished(false) {
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "Failed to load font!\n";
    }

    bottomPanel.setSize(sf::Vector2f(780.f, 180.f));
    bottomPanel.setPosition(sf::Vector2f(10.f, 400.f));
    bottomPanel.setFillColor(sf::Color::White);
    bottomPanel.setOutlineColor(sf::Color::Black);
    bottomPanel.setOutlineThickness(5.f);

    pHealthBarBg.setSize(sf::Vector2f(200.f, 15.f));
    pHealthBarBg.setFillColor(sf::Color::Red);
    pHealthBar = pHealthBarBg;
    pHealthBar.setFillColor(sf::Color::Green);
    pHealthBarBg.setPosition(sf::Vector2f(500.f, 350.f));
    pHealthBar.setPosition(sf::Vector2f(500.f, 350.f));

    eHealthBarBg.setSize(sf::Vector2f(200.f, 15.f));
    eHealthBarBg.setFillColor(sf::Color::Red);
    eHealthBar = eHealthBarBg;
    eHealthBar.setFillColor(sf::Color::Green);
    eHealthBarBg.setPosition(sf::Vector2f(50.f, 90.f));
    eHealthBar.setPosition(sf::Vector2f(50.f, 90.f));

    pushEvent("Trainer " + enemy->name + " challenges you!\nGo! " + player->getActivePokemon()->name + "!");
    updateHealthBars();
}

void Battle::pushEvent(const std::string& msg, std::function<void()> action) {
    eventQueue.push_back({msg, action});
}

void Battle::run(sf::RenderWindow& window) {
    while (window.isOpen() && !isFinished) {
        processEvents(window);
        update();
        render(window);
    }
}

void Battle::resolveTurn(int playerMoveIndex, bool usedItemOrSwitched, int pHealAmount) {
    Pokemon* pMon = player->getActivePokemon();
    Pokemon* eMon = enemy->getActivePokemon();
    
    if (!pMon || !eMon) return;

    int pSimHp = pMon->current_hp + pHealAmount;
    if (pSimHp > pMon->max_hp) pSimHp = pMon->max_hp;
    
    int eSimHp = eMon->current_hp;

    int enemyMoveIndex = std::rand() % static_cast<int>(eMon->moves.size());
    bool enemyUsedPotion = false;
    
    // Enemy AI: 50% chance to use a potion if HP <= 30%
    if (enemy->potions > 0 && eSimHp <= eMon->max_hp * 0.3f && (std::rand() % 100 < 50)) {
        enemyUsedPotion = true;
    }

    auto simulateMove = [&](Pokemon* attacker, Pokemon* defender, Move move, int& defHp) {
        std::string msg = attacker->name + " used " + move.name + "!";
        int hitChance = std::rand() % 100 + 1;
        int damage = 0;
        if (hitChance <= move.accuracy) {
            if (move.power > 0) {
                float stab = (attacker->type1 == move.type || attacker->type2 == move.type) ? 1.5f : 1.0f;
                float effectiveness = getTypeEffectiveness(move.type, defender->type1) * getTypeEffectiveness(move.type, defender->type2);
                
                damage = static_cast<int>((((2.0f * 5.0f / 5.0f + 2.0f) * move.power * ((float)attacker->attack / defender->defense)) / 50.0f + 2.0f) * stab * effectiveness);
                if (damage < 1) damage = 1;
                
                if (effectiveness > 1.0f) msg += "\nIt's super effective!";
                else if (effectiveness < 1.0f && effectiveness > 0.0f) msg += "\nIt's not very effective...";
                else if (effectiveness == 0.0f) msg += "\nIt had no effect!";
            } else {
                damage = 0;
                if (move.name == "Splash" || move.name == "Teleport") {
                    msg += "\nBut nothing happened!";
                }
            }
        } else {
            msg += "\nBut it missed!";
        }

        defHp -= damage;
        pushEvent(msg, [defender, damage]() {
            defender->takeDamage(damage);
        });
    };

    auto simulateEnemyPotion = [&]() {
        enemy->potions--;
        int healed = std::min(20, eMon->max_hp - eSimHp);
        eSimHp += healed;
        pushEvent("Trainer " + enemy->name + " used a Potion!\n" + eMon->name + " recovered " + std::to_string(healed) + " HP!", [eMon, this]() {
            eMon->heal(20);
        });
    };

    // Determine turn order
    if (usedItemOrSwitched) {
        if (enemyUsedPotion) {
            simulateEnemyPotion();
        } else {
            simulateMove(eMon, pMon, eMon->moves[enemyMoveIndex], pSimHp);
        }
    } else {
        if (enemyUsedPotion) {
            simulateEnemyPotion();
            simulateMove(pMon, eMon, pMon->moves[playerMoveIndex], eSimHp);
        } else {
            bool playerGoesFirst = pMon->speed >= eMon->speed;
            if (playerGoesFirst) {
                simulateMove(pMon, eMon, pMon->moves[playerMoveIndex], eSimHp);
                if (eSimHp > 0) {
                    simulateMove(eMon, pMon, eMon->moves[enemyMoveIndex], pSimHp);
                }
            } else {
                simulateMove(eMon, pMon, eMon->moves[enemyMoveIndex], pSimHp);
                if (pSimHp > 0) {
                    simulateMove(pMon, eMon, pMon->moves[playerMoveIndex], eSimHp);
                }
            }
        }
    }
    
    currentState = BattleState::Action_Text;
    
    // Execute the first event's action immediately so it matches the text on screen
    if (!eventQueue.empty() && eventQueue.front().action) {
        eventQueue.front().action();
        eventQueue.front().action = nullptr; // prevent double execution
    }
}

void Battle::processEvents(sf::RenderWindow& window) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Z) {
                if (currentState == BattleState::Action_Text || currentState == BattleState::End) {
                    if (!eventQueue.empty()) {
                        eventQueue.erase(eventQueue.begin());
                        
                        // Execute the next action in the queue as it comes up
                        if (!eventQueue.empty() && eventQueue.front().action) {
                            eventQueue.front().action();
                            eventQueue.front().action = nullptr;
                        }
                    }
                    if (eventQueue.empty()) {
                        if (currentState == BattleState::End) {
                            isFinished = true;
                        } else if (player->hasAlivePokemon() && enemy->hasAlivePokemon()) {
                            currentState = BattleState::PlayerTurn_Main;
                            selectedMenuIndex = 0;
                        }
                    }
                } else if (currentState == BattleState::PlayerTurn_Main) {
                    if (selectedMenuIndex == 0) {
                        currentState = BattleState::PlayerTurn_MoveSelect; // Fight
                        selectedMenuIndex = 0;
                    } else if (selectedMenuIndex == 1) { // Potion
                        if (player->potions > 0) {
                            player->potions--;
                            Pokemon* pMon = player->getActivePokemon();
                            int healed = std::min(20, pMon->max_hp - pMon->current_hp);
                            
                            pushEvent("You used a Potion!\n" + pMon->name + " recovered " + std::to_string(healed) + " HP!", [pMon]() {
                                pMon->heal(20);
                            });
                            resolveTurn(-1, true, healed);
                            selectedMenuIndex = 0;
                        }
                    } else if (selectedMenuIndex == 2) {
                        currentState = BattleState::PlayerTurn_SwitchSelect; // Pokemon
                        selectedMenuIndex = 0;
                    }
                } else if (currentState == BattleState::PlayerTurn_MoveSelect) {
                    Pokemon* pMon = player->getActivePokemon();
                    if (pMon && selectedMenuIndex >= 0 && selectedMenuIndex < pMon->moves.size()) {
                        resolveTurn(selectedMenuIndex, false);
                    }
                } else if (currentState == BattleState::PlayerTurn_SwitchSelect) {
                    if (selectedMenuIndex == player->party.size()) {
                        // Cancel
                        currentState = BattleState::PlayerTurn_Main;
                        selectedMenuIndex = 0;
                    } else if (player->switchPokemon(selectedMenuIndex)) {
                        pushEvent("You sent out " + player->getActivePokemon()->name + "!");
                        resolveTurn(-1, true);
                    }
                }
            } 
            else if (keyPressed->code == sf::Keyboard::Key::Up) {
                selectedMenuIndex--;
                if(selectedMenuIndex < 0) selectedMenuIndex = 0;
            } else if (keyPressed->code == sf::Keyboard::Key::Down) {
                selectedMenuIndex++;
                int maxIndex = 3;
                if (currentState == BattleState::PlayerTurn_SwitchSelect) {
                    maxIndex = static_cast<int>(player->party.size()); // To include "Cancel"
                } else if (currentState == BattleState::PlayerTurn_MoveSelect) {
                    if (player->getActivePokemon()) {
                        maxIndex = static_cast<int>(player->getActivePokemon()->moves.size()) - 1;
                    }
                } else if (currentState == BattleState::PlayerTurn_Main) {
                    maxIndex = 2; // Fight, Potion, Pokemon
                }
                if(selectedMenuIndex > maxIndex) selectedMenuIndex = maxIndex;
            } 
            else if (keyPressed->code == sf::Keyboard::Key::Escape || keyPressed->code == sf::Keyboard::Key::X) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect) {
                    currentState = BattleState::PlayerTurn_Main;
                    selectedMenuIndex = 0;
                }
            }
        }
    }
}

void Battle::update() {
    updateHealthBars();

    if (currentState == BattleState::Action_Text && eventQueue.empty()) {
        if (!player->hasAlivePokemon()) {
            pushEvent("You blacked out!\n\nPress Z or Enter to restart");
            currentState = BattleState::End;
        } else if (!enemy->hasAlivePokemon()) {
            pushEvent("You won!\n\nPress Z or Enter to restart");
            currentState = BattleState::End;
        } else if (!player->getActivePokemon()) {
            // Player's active pokemon died, force switch
            currentState = BattleState::PlayerTurn_SwitchSelect;
        } else if (!enemy->getActivePokemon()) {
            // Enemy's active pokemon died, auto switch (for now it just picks the next alive)
            for (int i = 0; i < enemy->party.size(); ++i) {
                if (enemy->party[i].isAlive()) {
                    enemy->switchPokemon(i);
                    pushEvent("Trainer " + enemy->name + " sent out " + enemy->getActivePokemon()->name + "!");
                    currentState = BattleState::Action_Text;
                    
                    // Execute immediately so it displays
                    if (!eventQueue.empty() && eventQueue.front().action) {
                        eventQueue.front().action();
                        eventQueue.front().action = nullptr;
                    }
                    
                    break;
                }
            }
        }
    }
}

void Battle::updateHealthBars() {
    Pokemon* pMon = player->getActivePokemon();
    if (pMon) {
        float pRatio = (float)pMon->current_hp / pMon->max_hp;
        pHealthBar.setSize(sf::Vector2f(200.f * pRatio, 15.f));
    }
    
    Pokemon* eMon = enemy->getActivePokemon();
    if (eMon) {
        float eRatio = (float)eMon->current_hp / eMon->max_hp;
        eHealthBar.setSize(sf::Vector2f(200.f * eRatio, 15.f));
    }
}

void Battle::render(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 240, 240));

    Pokemon* eMon = enemy->getActivePokemon();
    if (eMon) {
        sf::Text eText(font, eMon->name + "  Lvl " + std::to_string(eMon->level), 24);
        eText.setFillColor(sf::Color::Black);
        eText.setPosition(sf::Vector2f(50.f, 50.f));
        window.draw(eText);
        window.draw(eHealthBarBg);
        window.draw(eHealthBar);

        // Placeholder for Enemy Sprite (Top-Right)
        sf::Text eSpriteText(font, "[ " + eMon->name + " Sprite ]", 28);
        eSpriteText.setFillColor(sf::Color(100, 100, 100)); // Dark grey
        eSpriteText.setPosition(sf::Vector2f(500.f, 80.f));
        window.draw(eSpriteText);
    }

    Pokemon* pMon = player->getActivePokemon();
    if (pMon) {
        sf::Text pText(font, pMon->name + "  Lvl " + std::to_string(pMon->level) + "\nHP: " + std::to_string(pMon->current_hp) + "/" + std::to_string(pMon->max_hp), 24);
        pText.setFillColor(sf::Color::Black);
        pText.setPosition(sf::Vector2f(500.f, 280.f));
        window.draw(pText);
        window.draw(pHealthBarBg);
        window.draw(pHealthBar);

        // Placeholder for Player Sprite (Bottom-Left)
        sf::Text pSpriteText(font, "[ " + pMon->name + " Sprite (Back) ]", 28);
        pSpriteText.setFillColor(sf::Color(100, 100, 100)); // Dark grey
        pSpriteText.setPosition(sf::Vector2f(80.f, 250.f));
        window.draw(pSpriteText);
    }

    window.draw(bottomPanel);

    sf::Text uiText(font, "", 24);
    uiText.setFillColor(sf::Color::Black);
    uiText.setPosition(sf::Vector2f(30.f, 420.f));

    if (currentState == BattleState::Action_Text || currentState == BattleState::End) {
        if (!eventQueue.empty()) {
            uiText.setString(eventQueue.front().message);
        }
    } else if (currentState == BattleState::PlayerTurn_Main) {
        uiText.setString("What will " + pMon->name + " do?\n\n" +
                         (selectedMenuIndex == 0 ? "> " : "  ") + "1. Fight\n" +
                         (selectedMenuIndex == 1 ? "> " : "  ") + "2. Potion (" + std::to_string(player->potions) + " left)\n" +
                         (selectedMenuIndex == 2 ? "> " : "  ") + "3. Pokemon");
    } else if (currentState == BattleState::PlayerTurn_MoveSelect) {
        std::string menu = "Choose Move:\n";
        if (pMon) {
            for (int i = 0; i < pMon->moves.size(); ++i) {
                menu += (selectedMenuIndex == i ? "> " : "  ") + pMon->moves[i].name + "\n";
            }
        }
        uiText.setString(menu);
    } else if (currentState == BattleState::PlayerTurn_SwitchSelect) {
        std::string menu = "Choose Pokemon:\n";
        for (int i = 0; i < player->party.size(); ++i) {
            menu += (selectedMenuIndex == i ? "> " : "  ") + player->party[i].name + " (HP: " + std::to_string(player->party[i].current_hp) + ")\n";
        }
        menu += (selectedMenuIndex == player->party.size() ? "> " : "  ") + std::string("Cancel\n");
        uiText.setString(menu);
    }

    window.draw(uiText);
    window.display();
}
