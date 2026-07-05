#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "GameState.h"
#include "../states/StartMenuState.h"
#include "../states/StoryState.h"
#include "../states/PlayingState.h"
#include "../states/GroundingState.h"
#include "../states/GuideState.h"
#include "../states/SettingsState.h"
#include "../ui/Button.h"


class Game
{
public:
    Game();
    void run();

private:
    enum class Screen { StartMenu, Story, Playing, Grounding, Guide };

    void processEvents();
    void update(float deltaTime);
    void render();

    void checkForScreenTransitions();
    void switchTo(Screen screen);
    GameState& activeState();

    void openSettings();
    bool cornerSettingsAvailable() const; 
    void applyMusicVolume();              

    sf::RenderWindow window;
    sf::Clock clock;

    std::shared_ptr<sf::Font> font;

    StartMenuState startMenuState;
    StoryState storyState;
    PlayingState playingState;
    GroundingState groundingState;
    GuideState guideState;

    sf::Music music;
    

    SettingsState settings;
    bool settingsOpen = false;
    std::shared_ptr<sf::Texture> settingsCornerTexture, settingsCornerPressedTexture;
    Button settingsCornerButton;

    Screen currentScreen = Screen::StartMenu;
};