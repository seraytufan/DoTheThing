#include "ObjectSpawner.h"

void ObjectSpawner::init(std::shared_ptr<sf::Texture> spawnTexture, float spriteScale, sf::Vector2f position,
    float initialInterval, std::function<float()> respawnIntervalFn)
{
    texture = std::move(spawnTexture);
    scale = spriteScale;
    spawnPosition = position;
    nextIntervalFn = std::move(respawnIntervalFn);
    nextSpawn = initialInterval;
    spawnTimer = 0.f;
}

void ObjectSpawner::update(float deltaTime, float pixels, bool worldMoving, float offscreenX)
{
    if (worldMoving)
    {
        spawnTimer += deltaTime;

        for (auto& sprite : sprites)
        {
            sprite.move(-pixels, 0.f);
        }
    }

    if (spawnTimer >= nextSpawn)
    {
        spawnTimer = 0.f;
        nextSpawn = nextIntervalFn();

        sf::Sprite sprite;
        if (texture) sprite.setTexture(*texture);
        sprite.setScale(scale, scale);
        sprite.setPosition(spawnPosition);
        sprites.push_back(sprite);
    }

    for (int i = static_cast<int>(sprites.size()) - 1; i >= 0; --i)
    {
        if (sprites[i].getPosition().x < offscreenX)
        {
            sprites.erase(sprites.begin() + i);
        }
    }
}

void ObjectSpawner::removeAt(std::size_t index)
{
    if (index < sprites.size())
    {
        sprites.erase(sprites.begin() + static_cast<long>(index));
    }
}

void ObjectSpawner::draw(sf::RenderWindow& window)
{
    for (auto& sprite : sprites)
    {
        window.draw(sprite);
    }
}