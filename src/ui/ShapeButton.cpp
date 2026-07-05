#include "ShapeButton.h"

namespace
{
    sf::Color lighten(sf::Color c, int amount)
    {
        auto clampChannel = [](int v) { return static_cast<sf::Uint8>(v > 255 ? 255 : v); };
        return sf::Color(clampChannel(c.r + amount), clampChannel(c.g + amount), clampChannel(c.b + amount), c.a);
    }
}

void ShapeButton::init(const std::string& labelText, sf::Font& font, float centerX, float centerY,
    float width, float height, sf::Color fillColor, sf::Color outlineColor, unsigned int textSize)
{
    baseFill = fillColor;
    hoverFill = lighten(fillColor, 30);

    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width / 2.f, height / 2.f);
    shape.setPosition(centerX, centerY);
    shape.setFillColor(baseFill);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(3.f);

    label.setFont(font);
    label.setString(labelText);
    label.setCharacterSize(textSize);
    label.setFillColor(sf::Color::White);
    label.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    label.setPosition(centerX, centerY);
}

void ShapeButton::update(const sf::Vector2f& mousePos)
{
    shape.setFillColor(contains(mousePos) ? hoverFill : baseFill);
}

bool ShapeButton::contains(const sf::Vector2f& point) const
{
    return shape.getGlobalBounds().contains(point);
}

void ShapeButton::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(label);
}

void ShapeButton::setLabel(const std::string& newLabel)
{
    label.setString(newLabel);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    
}