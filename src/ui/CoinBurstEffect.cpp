#include "CoinBurstEffect.h"
#include "../core/ResourceManager.h"

void CoinBurstEffect::init(const std::string& texturePath, int gridCols, int gridRows, float frameTimeSeconds, float burstScale)
{
    texture = TextureManager::instance().acquire(texturePath);
    texture->setSmooth(true);

    cols = gridCols;
    rows = gridRows;
    frameCount = cols * rows;
    frameWidth = static_cast<int>(texture->getSize().x) / cols;
    frameHeight = static_cast<int>(texture->getSize().y) / rows;

    frameTime = frameTimeSeconds;
    scale = burstScale;
}

void CoinBurstEffect::trigger(sf::Vector2f position)
{
    Burst burst;
    burst.sprite.setTexture(*texture);
    burst.sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    burst.sprite.setOrigin(frameWidth / 2.f, frameHeight / 0.8f);
    burst.sprite.setPosition(position);
    burst.sprite.setScale(scale, scale);
    activeBursts.push_back(burst);
}

void CoinBurstEffect::update(float deltaTime)
{
    for (int i = static_cast<int>(activeBursts.size()) - 1; i >= 0; --i)
    {
        Burst& burst = activeBursts[i];
        burst.frameTimer += deltaTime;

        while (burst.frameTimer >= frameTime)
        {
            burst.frameTimer -= frameTime;
            burst.currentFrame++;
        }

        if (burst.currentFrame >= frameCount)
        {
            
            activeBursts.erase(activeBursts.begin() + i);
            continue;
        }

        int col = burst.currentFrame % cols;
        int row = burst.currentFrame / cols;
        burst.sprite.setTextureRect(sf::IntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight));
    }
}

void CoinBurstEffect::draw(sf::RenderWindow& window)
{
    for (auto& burst : activeBursts)
    {
        window.draw(burst.sprite);
    }
}