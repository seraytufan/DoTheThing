#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../ui/BoxTraceProgress.h"

enum class BreathPhase
{
    Inhale,
    HoldIn,
    Exhale,
    HoldOut,
    Finished
};

class Grounding
{
public:
    Grounding();

    void init();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void reset();

    bool isFinished() const { return finished; }

private:
    void setPhaseVisuals();
    void updateLabelScale();

    BreathPhase phase = BreathPhase::Inhale;
    float phaseTimer = 0.f;
    bool finished = false;

    std::shared_ptr<sf::Texture> holdBackgroundTexture;
    std::shared_ptr<sf::Texture> releaseBackgroundTexture;
    sf::Sprite backgroundSprite;

    std::shared_ptr<sf::Texture> inhaleLabelTexture;
    std::shared_ptr<sf::Texture> holdLabelTexture;
    std::shared_ptr<sf::Texture> exhaleLabelTexture;
    sf::Sprite labelSprite;
    float labelScale = 0.f;

    BoxTraceProgress boxProgress;
};