#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>


class SpritesheetPopEffect
{
public:
    void init(const std::string& texturePath, int gridCols, int gridRows,
        float frameTime, float growDuration, float holdDuration, float shrinkDuration,
        float targetScale, sf::Vector2f position);


    void start();

    void update(float deltaTime);
    bool isFinished() const { return phase == Phase::Done; }

    void draw(sf::RenderWindow& window);

private:
    enum class Phase { Growing, Holding, Shrinking, Done };

    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    int cols = 1, rows = 1;
    int frameWidth = 0, frameHeight = 0;
    int frameCount = 1;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameTime = 0.1f;

    float growDuration = 1.f, holdDuration = 1.f, shrinkDuration = 1.f;
    float targetScale = 1.f;
    float phaseTimer = 0.f;
    Phase phase = Phase::Done;
};