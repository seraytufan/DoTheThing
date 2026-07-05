#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../core/GameState.h"
#include "../ui/Button.h"
#include "../ui/TypewriterText.h"


class StoryState : public GameState
{
public:
    void init(sf::Font& fontRef);

    void onEnter() override; 
    void onExit() override; 
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

    
    bool wasFinished();

private:
    void goToPage(std::size_t pageIndex);
    void handleMousePress(const sf::Vector2f& mousePos);
    void handleMouseRelease();
    void handleClick(const sf::Vector2f& mousePos);
    void playClickSound();

    std::shared_ptr<sf::Texture> backgroundTexture;
    sf::Sprite backgroundSprite;

    std::shared_ptr<sf::Texture> skipTexture, skipPressedTexture;
    std::shared_ptr<sf::Texture> nextTexture, nextPressedTexture;
    Button skipButton;
    Button nextButton;

    std::shared_ptr<sf::SoundBuffer> clickBuffer;
    sf::Sound clickSound;

    std::shared_ptr<sf::SoundBuffer> typingSoundBuffer;
    sf::Sound typingSound;

    TypewriterText dialogueText;
    std::vector<std::string> pages;
    std::size_t currentPage = 0;

    bool finishedFlag = false;

    
    int pressedButton = -1;
};