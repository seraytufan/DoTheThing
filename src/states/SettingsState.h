#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../core/GameState.h"
#include "../ui/Button.h"
#include "../ui/Slider.h"


class SettingsState : public GameState
{
public:
    void init();

    void onEnter() override; 
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

   
    bool wasClosed();

private:
    sf::RectangleShape dimOverlay;

    std::shared_ptr<sf::Texture> panelTexture;
    sf::Sprite panelSprite;

    std::shared_ptr<sf::Texture> okayTexture, okayPressedTexture;
    Button okayButton;
    bool okayPressed = false;

    Slider soundSlider;
    Slider musicSlider;

    bool closedFlag = false;
};