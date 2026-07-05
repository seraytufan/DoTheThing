#include "Slider.h"
#include <algorithm>

void Slider::init(float x, float y, float width, float height,
    sf::Color trackColor, sf::Color fillColor, sf::Color knobColor,
    float initialValue01)
{
    posX = x;
    posY = y;
    barWidth = width;
    barHeight = height;


    bar.init(posX, posY, barWidth, barHeight, barHeight / 2.f, trackColor, fillColor);

    float knobRadius = barHeight * 0.9f;
    knob.setRadius(knobRadius);
    knob.setOrigin(knobRadius, knobRadius);
    knob.setPointCount(40);
    knob.setFillColor(knobColor);
    knob.setOutlineColor(sf::Color(255, 255, 255));
    knob.setOutlineThickness(2.f);

    setValue(initialValue01);
}

void Slider::setValue(float value01)
{
    currentValue = std::clamp(value01, 0.f, 1.f);
    bar.setProgress(currentValue);
    knob.setPosition(posX + barWidth * currentValue, posY + barHeight / 2.f);
}

float Slider::valueFromMouseX(float mouseX) const
{
    if (barWidth <= 0.f) return 0.f;
    return std::clamp((mouseX - posX) / barWidth, 0.f, 1.f);
}

bool Slider::hitTest(const sf::Vector2f& point) const
{
   
    float pad = barHeight; 
    return point.x >= posX - barHeight && point.x <= posX + barWidth + barHeight &&
        point.y >= posY - pad && point.y <= posY + barHeight + pad;
}

void Slider::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        if (hitTest(mousePos))
        {
            dragging = true;
            setValue(valueFromMouseX(mousePos.x));
        }
    }
    else if (event.type == sf::Event::MouseMoved && dragging)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        setValue(valueFromMouseX(mousePos.x));
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        dragging = false;
    }
}

void Slider::draw(sf::RenderWindow& window)
{
    bar.draw(window);
    window.draw(knob);
}