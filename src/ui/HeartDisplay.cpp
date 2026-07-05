#include "HeartDisplay.h"
#include "../core/ResourceManager.h"

void HeartDisplay::init(int totalLives, sf::Vector2f position, float width, float spacing,
    const std::string& fullHeartPath, const std::string& lostHeartPath)
{
    fullTexture = TextureManager::instance().acquire(fullHeartPath);
    fullTexture->setSmooth(true);
    lostTexture = TextureManager::instance().acquire(lostHeartPath);
    lostTexture->setSmooth(true);

    targetWidth = width;
    livesRemaining = totalLives;

    hearts.clear();
    for (int i = 0; i < totalLives; ++i)
    {
        sf::Sprite heart;
        applyTexture(heart, *fullTexture);
        heart.setPosition(position.x + static_cast<float>(i) * spacing, position.y);
        hearts.push_back(heart);
    }
}

void HeartDisplay::applyTexture(sf::Sprite& heart, const sf::Texture& texture) const
{
    heart.setTexture(texture, true);

    float nativeWidth = static_cast<float>(texture.getSize().x);
    float scale = (nativeWidth > 0.f) ? targetWidth / nativeWidth : 1.f;
    heart.setScale(scale, scale);
}

void HeartDisplay::setLivesRemaining(int lives)
{
    livesRemaining = lives;
    for (std::size_t i = 0; i < hearts.size(); ++i)
    {
        bool alive = static_cast<int>(i) < livesRemaining;
        sf::Vector2f pos = hearts[i].getPosition(); // preserved across texture swaps
        applyTexture(hearts[i], alive ? *fullTexture : *lostTexture);
        hearts[i].setPosition(pos);
    }
}

void HeartDisplay::draw(sf::RenderWindow& window)
{
    for (auto& heart : hearts)
    {
        window.draw(heart);
    }
}