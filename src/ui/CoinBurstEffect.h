#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>


class CoinBurstEffect
{
public:
    void init(const std::string& texturePath, int gridCols, int gridRows, float frameTime, float scale);

  
    void trigger(sf::Vector2f position);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    struct Burst
    {
        sf::Sprite sprite;
        int currentFrame = 0;
        float frameTimer = 0.f;
    };

    std::shared_ptr<sf::Texture> texture;
    int cols = 1, rows = 1;
    int frameWidth = 0, frameHeight = 0, frameCount = 1;
    float frameTime = 0.1f;
    float scale = 1.f;

    std::vector<Burst> activeBursts;
};