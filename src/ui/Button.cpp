#include "Button.h"

void Button::init(std::shared_ptr<sf::Texture> normalTex,
    std::shared_ptr<sf::Texture> pressedTex,
    float targetWidth, float x, float y)
{
    normalTexture = std::move(normalTex);
    pressedTexture = std::move(pressedTex);
    baseWidth = targetWidth;
    posX = x;
    posY = y;

    applyTexture(normalTexture, baseWidth);
    sprite.setPosition(posX, posY);
}

void Button::applyTexture(const std::shared_ptr<sf::Texture>& texture, float width)
{
    if (!texture) return;

    sprite.setTexture(*texture, true);

    float textureWidth = static_cast<float>(texture->getSize().x);
    if (textureWidth < 1.f) textureWidth = 1.f;

    float scale = width / textureWidth;
    sprite.setScale(scale, scale);
    sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
}

void Button::update(const sf::Vector2f& mousePos, float hoverShrink)
{
    applyTexture(normalTexture, baseWidth);

    if (contains(mousePos))
    {
        applyTexture(pressedTexture, baseWidth * hoverShrink);
    }
}

bool Button::contains(const sf::Vector2f& point) const
{
    return sprite.getGlobalBounds().contains(point);
}

sf::FloatRect Button::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}