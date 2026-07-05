#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../core/GameState.h"


class TodoPopupState : public GameState
{
public:
    void init();

    void onEnter() override; 
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

   
    bool wasFinished();

private:
    enum class Phase { Showing, Disappearing };

    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    float baseScale = 1.f;

    Phase phase = Phase::Showing;
    float timer = 0.f;

    bool finishedFlag = false;
};