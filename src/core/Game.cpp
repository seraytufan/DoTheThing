#include "Game.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "AudioSettings.h"

#include <cstdlib>
#include <ctime>

using namespace Constants;

Game::Game()
    : window(sf::VideoMode(kWindowWidth, kWindowHeight), kWindowTitle)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    font = FontManager::instance().acquire(kFontPath);
    if (font->getInfo().family.empty())
    {
        
        window.setTitle("COULD NOT OPEN FONT!");
    }

    startMenuState.init();
    storyState.init(*font);
    playingState.init(*font);
    groundingState.init(*font);
    guideState.init();
    settings.init();

    settingsCornerTexture = TextureManager::instance().acquire(kStartMenuSettingsPath);
    settingsCornerTexture->setSmooth(true);
    settingsCornerPressedTexture = TextureManager::instance().acquire(kStartMenuSettingsPressedPath);
    settingsCornerPressedTexture->setSmooth(true);
    settingsCornerButton.init(settingsCornerTexture, settingsCornerPressedTexture,
        kSettingsCornerWidth, kSettingsCornerPos.x, kSettingsCornerPos.y);
    


    if (music.openFromFile(kGameMusicPath))
    {
        music.setLoop(true);
        music.play();
        applyMusicVolume();
    }
}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (settingsOpen)
        {
            settings.handleEvent(event, window);
            continue;
        }

        
        activeState().handleEvent(event, window);

        if (cornerSettingsAvailable() &&
            event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            if (settingsCornerButton.contains(mousePos))
            {
                openSettings();
            }
        }
    }
}

void Game::update(float deltaTime)
{
    if (settingsOpen)
    {
        settings.update(deltaTime, window);
        if (settings.wasClosed())
        {
            settingsOpen = false;
        }
    }
    else
    {
        activeState().update(deltaTime, window);
        checkForScreenTransitions();

        if (cornerSettingsAvailable())
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            settingsCornerButton.update(mousePos, 0.9f);
        }
    }

    applyMusicVolume();

    
    sf::Time dt = sf::seconds(deltaTime);
    TextureManager::instance().update(dt);
    FontManager::instance().update(dt);
    SoundManager::instance().update(dt);
}

void Game::checkForScreenTransitions()
{
    switch (currentScreen)
    {
    case Screen::StartMenu:
        if (startMenuState.wasPlayClicked())
        {
            switchTo(Screen::Story);
        }
        else if (startMenuState.wasTutorialClicked())
        {
            switchTo(Screen::Guide);
        }
        else if (startMenuState.wasSettingsClicked())
        {
            openSettings(); 
        }
        break;

    case Screen::Story:
        if (storyState.wasFinished())
        {
            switchTo(Screen::Playing);
        }
        break;

    case Screen::Playing:
        if (playingState.wasGroundingRequested())
        {
            switchTo(Screen::Grounding);
        }
        else if (playingState.wasReturnToMenuRequested())
        {
            switchTo(Screen::StartMenu); // Menu button on a win/lose popup
        }
        break;

    case Screen::Grounding:
        if (groundingState.isFinished())
        {
            switchTo(Screen::Playing);
        }
        break;

    case Screen::Guide:
        if (guideState.wasClosed())
        {
            switchTo(Screen::StartMenu);
        }
        break;
    }
}

void Game::switchTo(Screen screen)
{
    activeState().onExit();
    currentScreen = screen;
    activeState().onEnter();
}

void Game::openSettings()
{
   
    
    settingsOpen = true;
    settings.onEnter();
}

bool Game::cornerSettingsAvailable() const
{
   
    return currentScreen == Screen::Playing ||
        currentScreen == Screen::Grounding ||
        currentScreen == Screen::Guide;
}

void Game::applyMusicVolume()
{
    float volume = kGameMusicVolume * AudioSettings::instance().musicVolume01;
    if (currentScreen == Screen::Grounding)
    {
        volume = 0.f; 
    }
    music.setVolume(volume);
}

GameState& Game::activeState()
{
    switch (currentScreen)
    {
    case Screen::StartMenu: return startMenuState;
    case Screen::Story:     return storyState;
    case Screen::Playing:   return playingState;
    case Screen::Grounding: return groundingState;
    case Screen::Guide:     return guideState;
    }
    return startMenuState; 
}

void Game::render()
{
    sf::Color backgroundColor = kPlayingBackgroundColor;
    if (currentScreen == Screen::StartMenu) backgroundColor = kStartMenuBackgroundColor;
    else if (currentScreen == Screen::Grounding) backgroundColor = kGroundingBackgroundColor;
   

    window.clear(backgroundColor);
    activeState().render(window);

  
    if (cornerSettingsAvailable() && !settingsOpen)
    {
        settingsCornerButton.draw(window);
    }

    if (settingsOpen)
    {
        settings.render(window);
    }

    window.display();
}