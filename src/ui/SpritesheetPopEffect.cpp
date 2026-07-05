#include "SpritesheetPopEffect.h"
#include "../core/ResourceManager.h"

void SpritesheetPopEffect::init(const std::string& texturePath, int gridCols, int gridRows,
    float frameTimeSeconds, float grow, float hold, float shrink, float scale, sf::Vector2f position)
{
    texture = TextureManager::instance().acquire(texturePath);
    texture->setSmooth(true);

    cols = gridCols;
    rows = gridRows;
    frameCount = cols * rows;
    frameWidth = static_cast<int>(texture->getSize().x) / cols;
    frameHeight = static_cast<int>(texture->getSize().y) / rows;

    frameTime = frameTimeSeconds;
    growDuration = grow;
    holdDuration = hold;
    shrinkDuration = shrink;
    targetScale = scale;

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(position);
}

void SpritesheetPopEffect::start()
{
    phase = Phase::Growing;
    phaseTimer = 0.f;
    frameTimer = 0.f;
    currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(0.f, 0.f);
}

void SpritesheetPopEffect::update(float deltaTime)
{
    if (phase == Phase::Done) return;

    frameTimer += deltaTime;
    while (frameTimer >= frameTime)
    {
        frameTimer -= frameTime;
        currentFrame = (currentFrame + 1) % frameCount;
        int col = currentFrame % cols;
        int row = currentFrame / cols;
        sprite.setTextureRect(sf::IntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight));
    }

    phaseTimer += deltaTime;

    if (phase == Phase::Growing)
    {
        float t = phaseTimer / growDuration;
        if (t > 1.f) t = 1.f;
        float scale = targetScale * t;
        sprite.setScale(scale, scale);
        if (t >= 1.f)
        {
            phase = Phase::Holding;
            phaseTimer = 0.f;
        }
    }
    else if (phase == Phase::Holding)
    {
        if (phaseTimer >= holdDuration)
        {
            phase = Phase::Shrinking;
            phaseTimer = 0.f;
        }
    }
    else if (phase == Phase::Shrinking)
    {
        float t = phaseTimer / shrinkDuration;
        if (t > 1.f) t = 1.f;
        float scale = targetScale * (1.f - t);
        sprite.setScale(scale, scale);
        if (t >= 1.f)
        {
            phase = Phase::Done;
        }
    }
}

void SpritesheetPopEffect::draw(sf::RenderWindow& window)
{
    if (phase != Phase::Done)
    {
        window.draw(sprite);
    }
}