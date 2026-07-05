#include "ParallaxLayer.h"
#include "../core/ResourceManager.h"

void ParallaxLayer::init(const std::string& texturePath, float yPosition, float targetHeight, float windowWidth)
{
    texture = TextureManager::instance().acquire(texturePath);
    texture->setRepeated(true);

    sprite.setTexture(*texture);
    sprite.setPosition(0.f, yPosition);

    sf::Vector2u size = texture->getSize();
    float scale = (size.y > 0) ? targetHeight / static_cast<float>(size.y) : 1.f;
    sprite.setScale(scale, scale);

   
    rectWidth = static_cast<int>(windowWidth / scale) + static_cast<int>(size.x);
    sprite.setTextureRect(sf::IntRect(0, 0, rectWidth, static_cast<int>(size.y)));

    scrollX = 0.f;
}

void ParallaxLayer::scroll(float pixels)
{
    scrollX += pixels;
    int rectX = static_cast<int>(scrollX);
    sf::IntRect rect = sprite.getTextureRect();
    sprite.setTextureRect(sf::IntRect(rectX, 0, rectWidth, rect.height));
}

void ParallaxLayer::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}