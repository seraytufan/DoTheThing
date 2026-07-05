#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


class Button
{
public:
    void init(std::shared_ptr<sf::Texture> normalTexture,
        std::shared_ptr<sf::Texture> pressedTexture,
        float targetWidth, float x, float y);

  
    void update(const sf::Vector2f& mousePos, float hoverShrink = 0.9f);

    bool contains(const sf::Vector2f& point) const;
    sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderWindow& window);

private:
    void applyTexture(const std::shared_ptr<sf::Texture>& texture, float width);

    std::shared_ptr<sf::Texture> normalTexture;
    std::shared_ptr<sf::Texture> pressedTexture;
    sf::Sprite sprite;

    float baseWidth = 0.f;
    float posX = 0.f, posY = 0.f;
};
