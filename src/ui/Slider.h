#pragma once

#include <SFML/Graphics.hpp>
#include "ProgressBar.h"


class Slider
{
public:
    void init(float x, float y, float width, float height,
        sf::Color trackColor, sf::Color fillColor, sf::Color knobColor,
        float initialValue01);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    float value() const { return currentValue; }
    void setValue(float value01);

    void draw(sf::RenderWindow& window);

private:
    bool hitTest(const sf::Vector2f& point) const;
    float valueFromMouseX(float mouseX) const;

    ProgressBar bar;
    sf::CircleShape knob;

    float posX = 0.f, posY = 0.f, barWidth = 0.f, barHeight = 0.f;
    float currentValue = 0.f;
    bool dragging = false;
};