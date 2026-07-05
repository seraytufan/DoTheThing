#pragma once

#include <SFML/Graphics.hpp>



class GameState
{
public:
    virtual ~GameState() = default;

    // Called once, right when this state becomes the active state.
    virtual void onEnter() {}

    // Called once, right before switching away from this state.
    virtual void onExit() {}

   
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window)
    {
        (void)event;
        (void)window;
    }

    
    virtual void update(float deltaTime, sf::RenderWindow& window) = 0;

    virtual void render(sf::RenderWindow& window) = 0;
};