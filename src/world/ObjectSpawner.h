#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <vector>

class ObjectSpawner
{
public:
    
    void init(std::shared_ptr<sf::Texture> texture, float scale, sf::Vector2f spawnPosition,
        float initialInterval, std::function<float()> respawnIntervalFn);

   
    void update(float deltaTime, float pixels, bool worldMoving, float offscreenX);

   
    std::vector<sf::Sprite>& items() { return sprites; }
    void removeAt(std::size_t index);

    void draw(sf::RenderWindow& window);

private:
    std::shared_ptr<sf::Texture> texture;
    float scale = 1.f;
    sf::Vector2f spawnPosition;

    std::function<float()> nextIntervalFn;
    float spawnTimer = 0.f;
    float nextSpawn = 0.f;

    std::vector<sf::Sprite> sprites;
};