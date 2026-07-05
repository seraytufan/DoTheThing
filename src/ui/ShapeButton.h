#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class ShapeButton
{
public:
    void init(const std::string& label, sf::Font& font, float centerX, float centerY,
        float width, float height, sf::Color fillColor, sf::Color outlineColor, unsigned int textSize);

    void update(const sf::Vector2f& mousePos);
    bool contains(const sf::Vector2f& point) const;
    void draw(sf::RenderWindow& window);


    void setLabel(const std::string& newLabel);

private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Color baseFill;
    sf::Color hoverFill;
};