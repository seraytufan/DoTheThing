#pragma once

#include <SFML/Graphics.hpp>

class CountdownTimer
{
public:
    void init(sf::Font& font, unsigned int characterSize, sf::Color color, float totalSeconds);
    void setPosition(float x, float y);

    
    void reset(float totalSeconds);

    void update(float deltaTime, bool isRunning);

    bool isExpired() const { return remainingSeconds <= 0.f; }
    float getRemainingSeconds() const { return remainingSeconds; }

    void draw(sf::RenderWindow& window);

private:
    void refreshDisplayString();

    sf::Text text;
    float remainingSeconds = 0.f;
};