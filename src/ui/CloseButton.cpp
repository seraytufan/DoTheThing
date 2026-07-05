#include "CloseButton.h"

void CloseButton::init(std::shared_ptr<sf::Texture> normalTex,
    std::shared_ptr<sf::Texture> pressedTex,
    float targetWidth, sf::Vector2f topRightCorner, sf::Vector2f inset)
{
    normalTexture = std::move(normalTex);
    pressedTexture = std::move(pressedTex);
    baseWidth = targetWidth;
    center = sf::Vector2f(topRightCorner.x - inset.x, topRightCorner.y + inset.y);

    applyTexture(normalTexture, baseWidth);
}

void CloseButton::applyTexture(const std::shared_ptr<sf::Texture>& texture, float width)
{
    if (!texture) return;

    sprite.setTexture(*texture, true);

    float textureWidth = static_cast<float>(texture->getSize().x);
    if (textureWidth < 1.f) textureWidth = 1.f;

    float scale = width / textureWidth;
    sprite.setScale(scale, scale);
    sprite.setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
    sprite.setPosition(center); 
}

void CloseButton::update(const sf::Vector2f& mousePos)
{
    
    applyTexture(contains(mousePos) ? pressedTexture : normalTexture, baseWidth);
}

void CloseButton::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        pressed = contains(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        
        if (pressed && contains(mousePos))
        {
            clickedFlag = true;
        }
        pressed = false;
    }
}

bool CloseButton::contains(const sf::Vector2f& point) const
{
    return sprite.getGlobalBounds().contains(point);
}

bool CloseButton::wasClicked()
{
    if (clickedFlag)
    {
        clickedFlag = false;
        return true;
    }
    return false;
}

void CloseButton::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}