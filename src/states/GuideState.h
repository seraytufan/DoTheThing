#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "../core/GameState.h"
#include "../ui/CloseButton.h"

class GuideState : public GameState
{
public:
    void init();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

  
    bool wasClosed();

private:
    void playClickSound();

    std::shared_ptr<sf::Texture> menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    sf::RectangleShape dimOverlay;

    std::shared_ptr<sf::Texture> guideTexture;
    sf::Sprite guideSprite;

    std::shared_ptr<sf::Texture> closeTexture, closePressedTexture;
    CloseButton closeButton;

    std::shared_ptr<sf::SoundBuffer> clickBuffer;
    sf::Sound clickSound;

    bool closedFlag = false;
};