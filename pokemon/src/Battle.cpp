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
    updateSprites();
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

void Battle::simulateMove(Pokemon* attacker, Pokemon* defender, Move move, int& defHp) {
    std::string msg = attacker->name + " used " + move.name + "!";
    int hitChance = std::rand() % 100 + 1;
    int damage = 0;
    float accMult = attacker->getAccuracyMultiplier();
    float evaMult = defender->getEvasionMultiplier();
    if (hitChance <= move.accuracy * (accMult / evaMult)) {
        if (move.power > 0) {
            float stab = (attacker->type1 == move.type || attacker->type2 == move.type) ? 1.5f : 1.0f;
            float effectiveness = getTypeEffectiveness(move.type, defender->type1) * getTypeEffectiveness(move.type, defender->type2);
            
            damage = static_cast<int>((((2.0f * 5.0f / 5.0f + 2.0f) * move.power * ((float)attacker->getAttack() / defender->getDefense())) / 50.0f + 2.0f) * stab * effectiveness);
            if (damage < 1) damage = 1;
            
            if (effectiveness > 1.0f) msg += "\nIt's super effective!";
            else if (effectiveness < 1.0f && effectiveness > 0.0f) msg += "\nIt's not very effective...";
            else if (effectiveness == 0.0f) msg += "\nIt had no effect!";
        } else {
            damage = 0;
            if (move.statTarget != Stat::None) {
                Pokemon* statTargetMon = move.targetSelf ? attacker : defender;
                int* stagePtr = nullptr;
                std::string statName = "";
                if (move.statTarget == Stat::Attack) { stagePtr = &statTargetMon->attack_stage; statName = "Attack"; }
                else if (move.statTarget == Stat::Defense) { stagePtr = &statTargetMon->defense_stage; statName = "Defense"; }
                else if (move.statTarget == Stat::Speed) { stagePtr = &statTargetMon->speed_stage; statName = "Speed"; }
                else if (move.statTarget == Stat::Accuracy) { stagePtr = &statTargetMon->accuracy_stage; statName = "Accuracy"; }
                else if (move.statTarget == Stat::Evasion) { stagePtr = &statTargetMon->evasion_stage; statName = "Evasion"; }
                
                if (stagePtr) {
                    int oldStage = *stagePtr;
                    *stagePtr += move.statChange;
                    if (*stagePtr > 6) *stagePtr = 6;
                    if (*stagePtr < -6) *stagePtr = -6;
                    
                    if (*stagePtr == oldStage) {
                        msg += "\n" + statTargetMon->name + "'s " + statName + " won't go any " + (move.statChange > 0 ? "higher!" : "lower!");
                    } else {
                        if (move.statChange > 1) msg += "\n" + statTargetMon->name + "'s " + statName + " sharply rose!";
                        else if (move.statChange > 0) msg += "\n" + statTargetMon->name + "'s " + statName + " rose!";
                        else if (move.statChange < -1) msg += "\n" + statTargetMon->name + "'s " + statName + " harshly fell!";
                        else msg += "\n" + statTargetMon->name + "'s " + statName + " fell!";
                    }
                }
            } else if (move.name == "Splash" || move.name == "Teleport") {
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
}

void Battle::simulateEnemyPotion(Pokemon* eMon, int& eSimHp) {
    enemy->potions--;
    int healed = std::min(20, eMon->max_hp - eSimHp);
    eSimHp += healed;
    pushEvent("Trainer " + enemy->name + " used a Potion!\n" + eMon->name + " recovered " + std::to_string(healed) + " HP!", [eMon, this]() {
        eMon->heal(20);
    });
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
    
    // Enemy trainer 50% chance to use a potion if HP <= 30%
    if (enemy->potions > 0 && eSimHp <= eMon->max_hp * 0.3f && (std::rand() % 100 < 50)) {
        enemyUsedPotion = true;
    }

    // Determine turn order
    if (usedItemOrSwitched) {
        if (enemyUsedPotion) {
            simulateEnemyPotion(eMon, eSimHp);
        } else {
            simulateMove(eMon, pMon, eMon->moves[enemyMoveIndex], pSimHp);
        }
    } else {
        if (enemyUsedPotion) {
            simulateEnemyPotion(eMon, eSimHp);
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
    
    // Execute the first events action immediately so it matches the text on screen
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
                        } else if (!player->hasAlivePokemon()) {
                            pushEvent("You blacked out!\n\nPress Z or Enter to restart");
                            currentState = BattleState::End;
                        } else if (!enemy->hasAlivePokemon()) {
                            pushEvent("You won!\n\nPress Z or Enter to restart");
                            currentState = BattleState::End;
                        } else if (!player->getActivePokemon()) {
                            currentState = BattleState::PlayerTurn_SwitchSelect;
                        } else if (!enemy->getActivePokemon()) {
                            for (int i = 0; i < enemy->party.size(); ++i) {
                                if (enemy->party[i].isAlive()) {
                                    enemy->switchPokemon(i);
                                    updateSprites();
                                    pushEvent("Trainer " + enemy->name + " sent out " + enemy->getActivePokemon()->name + "!");
                                    currentState = BattleState::Action_Text;
                                    break;
                                }
                            }
                        } else {
                            currentState = BattleState::PlayerTurn_Main;
                            selectedMenuIndex = 0;
                        }
                    }
                } else if (currentState == BattleState::PlayerTurn_Main) {
                    if (selectedMenuIndex == 0) {
                        currentState = BattleState::PlayerTurn_MoveSelect; // Fight
                        Pokemon* pMon = player->getActivePokemon();
                        if (pMon) {
                            selectedMenuIndex = pMon->lastMoveIndex;
                            if (selectedMenuIndex >= pMon->moves.size() && selectedMenuIndex != 5) {
                                selectedMenuIndex = 0;
                            }
                        } else {
                            selectedMenuIndex = 0;
                        }
                    } else if (selectedMenuIndex == 1) { // Potion
                        if (player->potions > 0) {
                            currentState = BattleState::PlayerTurn_PotionConfirm;
                            selectedMenuIndex = 0;
                        }
                    } else if (selectedMenuIndex == 2) {
                        currentState = BattleState::PlayerTurn_SwitchSelect; // Pokemon
                        selectedMenuIndex = 0;
                    }
                } else if (currentState == BattleState::PlayerTurn_PotionConfirm) {
                    if (selectedMenuIndex == 0) {
                        Pokemon* pMon = player->getActivePokemon();
                        if (pMon) {
                            player->potions--;
                            int healed = std::min(20, pMon->max_hp - pMon->current_hp);
                            
                            pushEvent("You used a Potion!\n" + pMon->name + " recovered " + std::to_string(healed) + " HP!", [pMon]() {
                                pMon->heal(20);
                            });
                            resolveTurn(-1, true, healed);
                        }
                        selectedMenuIndex = 0;
                    } else {
                        currentState = BattleState::PlayerTurn_Main;
                        selectedMenuIndex = 1;
                    }
                } else if (currentState == BattleState::PlayerTurn_MoveSelect) {
                    Pokemon* pMon = player->getActivePokemon();
                    if (pMon) {
                        if (selectedMenuIndex == 5) { // Cancel is index 5
                            currentState = BattleState::PlayerTurn_Main;
                            selectedMenuIndex = 0;
                        } else if (selectedMenuIndex >= 0 && selectedMenuIndex < pMon->moves.size()) {
                            pMon->lastMoveIndex = selectedMenuIndex;
                            resolveTurn(selectedMenuIndex, false);
                        }
                    }
                } else if (currentState == BattleState::PlayerTurn_SwitchSelect) {
                    if (selectedMenuIndex == 7) { // Cancel is index 7
                        // Cancel
                        if (player->getActivePokemon() != nullptr) {
                            currentState = BattleState::PlayerTurn_Main;
                            selectedMenuIndex = 0;
                        }
                    } else if (selectedMenuIndex >= 0 && selectedMenuIndex < player->party.size()) {
                        bool wasForcedSwitch = (player->getActivePokemon() == nullptr);
                        if (player->switchPokemon(selectedMenuIndex)) {
                            updateSprites();
                            pushEvent("You sent out " + player->getActivePokemon()->name + "!");
                            if (wasForcedSwitch) {
                                currentState = BattleState::Action_Text;
                            } else {
                                resolveTurn(-1, true);
                            }
                        }
                    }
                }
            } 
            else if (keyPressed->code == sf::Keyboard::Key::Up) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect) {
                    selectedMenuIndex -= 2;
                } else {
                    selectedMenuIndex--;
                }
                if (selectedMenuIndex < 0) selectedMenuIndex = 0;
            } else if (keyPressed->code == sf::Keyboard::Key::Down) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect) {
                    selectedMenuIndex += 2;
                } else {
                    selectedMenuIndex++;
                }
                
                int maxIndex = 3;
                if (currentState == BattleState::PlayerTurn_SwitchSelect) maxIndex = 7;
                else if (currentState == BattleState::PlayerTurn_MoveSelect && player->getActivePokemon()) maxIndex = 5;
                else if (currentState == BattleState::PlayerTurn_PotionConfirm) maxIndex = 1;
                else if (currentState == BattleState::PlayerTurn_Main) maxIndex = 2;
                
                if (selectedMenuIndex > maxIndex) selectedMenuIndex = maxIndex;
            }
            else if (keyPressed->code == sf::Keyboard::Key::Left) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect) {
                    if (selectedMenuIndex % 2 != 0) selectedMenuIndex--;
                }
            }
            else if (keyPressed->code == sf::Keyboard::Key::Right) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect) {
                    if (selectedMenuIndex % 2 == 0) {
                        selectedMenuIndex++;
                        
                        int maxIndex = 3;
                        if (currentState == BattleState::PlayerTurn_SwitchSelect) maxIndex = 7;
                        else if (currentState == BattleState::PlayerTurn_MoveSelect && player->getActivePokemon()) maxIndex = 5;
                        
                        if (selectedMenuIndex > maxIndex) selectedMenuIndex = maxIndex;
                    }
                }
            } 
            else if (keyPressed->code == sf::Keyboard::Key::Escape || keyPressed->code == sf::Keyboard::Key::X) {
                if (currentState == BattleState::PlayerTurn_MoveSelect || currentState == BattleState::PlayerTurn_SwitchSelect || currentState == BattleState::PlayerTurn_PotionConfirm) {
                    currentState = BattleState::PlayerTurn_Main;
                    selectedMenuIndex = 0;
                }
            }
        }
    }
}

void Battle::update() {
    updateHealthBars();
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

        if (enemySprite) {
            window.draw(*enemySprite);
        }
    }

    Pokemon* pMon = player->getActivePokemon();
    if (pMon) {
        sf::Text pText(font, pMon->name + "  Lvl " + std::to_string(pMon->level) + "\nHP: " + std::to_string(pMon->current_hp) + "/" + std::to_string(pMon->max_hp), 24);
        pText.setFillColor(sf::Color::Black);
        pText.setPosition(sf::Vector2f(500.f, 280.f));
        window.draw(pText);
        window.draw(pHealthBarBg);
        window.draw(pHealthBar);

        if (playerSprite) {
            window.draw(*playerSprite);
        }
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
        std::string pName = pMon ? pMon->name : "Pokemon";
        uiText.setString("What will " + pName + " do?\n\n" +
                         (selectedMenuIndex == 0 ? "> " : "  ") + "1. Fight\n" +
                         (selectedMenuIndex == 1 ? "> " : "  ") + "2. Potion (" + std::to_string(player->potions) + " left)\n" +
                         (selectedMenuIndex == 2 ? "> " : "  ") + "3. Pokemon");
    } else if (currentState == BattleState::PlayerTurn_PotionConfirm) {
        std::string pName = pMon ? pMon->name : "Pokemon";
        uiText.setString("Use a Potion on " + pName + "?\n\n" +
                         (selectedMenuIndex == 0 ? "> " : "  ") + "Yes\n" +
                         (selectedMenuIndex == 1 ? "> " : "  ") + "No");
    } else if (currentState == BattleState::PlayerTurn_MoveSelect) {
        uiText.setString("Choose Move:");
        window.draw(uiText);
        
        if (pMon) {
            for (int i = 0; i < 4; ++i) {
                std::string mName = "-";
                if (i < pMon->moves.size()) {
                    mName = pMon->moves[i].name;
                }
                sf::Text moveText(font, (selectedMenuIndex == i ? "> " : "  ") + mName, 24);
                moveText.setFillColor(sf::Color::Black);
                float x = 30.f + (i % 2) * 350.f;
                float y = 460.f + (i / 2) * 30.f;
                moveText.setPosition(sf::Vector2f(x, y));
                window.draw(moveText);
            }
            int cancelIdx = 5; // Fixed position at bottom right
            sf::Text cancelText(font, (selectedMenuIndex == cancelIdx ? "> " : "  ") + std::string("Cancel"), 24);
            cancelText.setFillColor(sf::Color::Black);
            float cx = 30.f + (cancelIdx % 2) * 350.f;
            float cy = 460.f + (cancelIdx / 2) * 30.f;
            cancelText.setPosition(sf::Vector2f(cx, cy));
            window.draw(cancelText);
        }
        uiText.setString(""); // Clear so it doesn't draw twice
    } else if (currentState == BattleState::PlayerTurn_SwitchSelect) {
        uiText.setString("Choose Pokemon:");
        window.draw(uiText);
        
        for (int i = 0; i < 6; ++i) {
            std::string pkmnInfo = "-";
            if (i < player->party.size()) {
                pkmnInfo = player->party[i].name + " (" + std::to_string(player->party[i].current_hp) + ")";
            }
            sf::Text pkmnText(font, (selectedMenuIndex == i ? "> " : "  ") + pkmnInfo, 24);
            pkmnText.setFillColor(sf::Color::Black);
            float x = 30.f + (i % 2) * 350.f;
            float y = 460.f + (i / 2) * 30.f;
            pkmnText.setPosition(sf::Vector2f(x, y));
            window.draw(pkmnText);
        }
        int cancelIdx = 7; // Fixed position at bottom right
        sf::Text cancelText(font, (selectedMenuIndex == cancelIdx ? "> " : "  ") + std::string("Cancel"), 24);
        cancelText.setFillColor(sf::Color::Black);
        float cx = 30.f + (cancelIdx % 2) * 350.f;
        float cy = 460.f + (cancelIdx / 2) * 30.f;
        cancelText.setPosition(sf::Vector2f(cx, cy));
        window.draw(cancelText);
        
        uiText.setString(""); // Clear so it doesn't draw twice
    }

    if (uiText.getString() != "") {
        window.draw(uiText);
    }
    window.display();
}

void Battle::updateSprites() {
    Pokemon* pMon = player->getActivePokemon();
    if (pMon) {
        std::string pName = pMon->name;
        std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
        if (playerTexture.loadFromFile("assets/back/" + pName + ".png")) {
            playerSprite = sf::Sprite(playerTexture);
            playerSprite->setScale(sf::Vector2f(3.f, 3.f));
            playerSprite->setPosition(sf::Vector2f(100.f, 150.f));
        }
    }

    Pokemon* eMon = enemy->getActivePokemon();
    if (eMon) {
        std::string eName = eMon->name;
        std::transform(eName.begin(), eName.end(), eName.begin(), ::tolower);
        if (enemyTexture.loadFromFile("assets/front/" + eName + ".png")) {
            enemySprite = sf::Sprite(enemyTexture);
            enemySprite->setScale(sf::Vector2f(2.5f, 2.5f));
            enemySprite->setPosition(sf::Vector2f(500.f, 30.f));
        }
    }
}
