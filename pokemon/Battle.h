#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include "Trainer.h"

enum class BattleState {
    PlayerTurn_Main,
    PlayerTurn_MoveSelect,
    PlayerTurn_SwitchSelect,
    Action_Text, // displaying text like "Charmander used Scratch!"
    Check_Faint, // check if anyone fainted
    End // battle over
};

class Battle {
private:
    Trainer* player;
    Trainer* enemy;
    sf::Font font;
    BattleState currentState;
    bool isFinished;
    
    struct BattleEvent {
        std::string message;
        std::function<void()> action;
    };
    std::vector<BattleEvent> eventQueue;
    int selectedMenuIndex;
    
    // UI Rectangles
    sf::RectangleShape bottomPanel;
    sf::RectangleShape pHealthBarBg;
    sf::RectangleShape pHealthBar;
    sf::RectangleShape eHealthBarBg;
    sf::RectangleShape eHealthBar;
    
    // Sprites
    sf::Texture playerTexture;
    std::optional<sf::Sprite> playerSprite;
    sf::Texture enemyTexture;
    std::optional<sf::Sprite> enemySprite;

    // Helpers
    void updateSprites();
    
    void processEvents(sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);

    void updateHealthBars();
    void pushEvent(const std::string& msg, std::function<void()> action = nullptr);
    void resolveTurn(int playerMoveIndex, bool usedItemOrSwitched, int pHealAmount = 0);

public:
    Battle(Trainer* p, Trainer* e);
    void run(sf::RenderWindow& window);
};
