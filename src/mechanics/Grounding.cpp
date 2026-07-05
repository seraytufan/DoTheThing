#include "Grounding.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"

using namespace Constants;

Grounding::Grounding() = default;

void Grounding::init()
{
    holdBackgroundTexture = TextureManager::instance().acquire(kGroundingHoldBackgroundPath);
    releaseBackgroundTexture = TextureManager::instance().acquire(kGroundingReleaseBackgroundPath);
    backgroundSprite.setTexture(*holdBackgroundTexture); 

    inhaleLabelTexture = TextureManager::instance().acquire(kGroundingInhaleLabelPath);
    inhaleLabelTexture->setSmooth(true);
    holdLabelTexture = TextureManager::instance().acquire(kGroundingHoldLabelPath);
    holdLabelTexture->setSmooth(true);
    exhaleLabelTexture = TextureManager::instance().acquire(kGroundingExhaleLabelPath);
    exhaleLabelTexture->setSmooth(true);

    labelSprite.setPosition(kGroundingLabelPos);

    boxProgress.init(kGroundingBoxPos.x, kGroundingBoxPos.y, kGroundingBoxSize, kGroundingBoxSize,
        kGroundingBoxInset, kGroundingBoxBorderThickness, kGroundingBoxCornerRadius, kGroundingBoxProgressColor);

    setPhaseVisuals();
    labelScale = kGroundingLabelMinScale;
    labelSprite.setScale(labelScale, labelScale);
}

void Grounding::setPhaseVisuals()
{
    switch (phase)
    {
    case BreathPhase::Inhale:
        backgroundSprite.setTexture(*holdBackgroundTexture);
        labelSprite.setTexture(*inhaleLabelTexture, true);
        break;
    case BreathPhase::HoldIn:
        backgroundSprite.setTexture(*releaseBackgroundTexture);
        labelSprite.setTexture(*holdLabelTexture, true);
        break;
    case BreathPhase::Exhale:
        backgroundSprite.setTexture(*holdBackgroundTexture);
        labelSprite.setTexture(*exhaleLabelTexture, true);
        break;
    case BreathPhase::HoldOut:
        backgroundSprite.setTexture(*releaseBackgroundTexture);
        labelSprite.setTexture(*holdLabelTexture, true);
        break;
    default:
        break;
    }

    
    sf::Vector2u size = labelSprite.getTexture()->getSize();
    labelSprite.setOrigin(size.x / 2.f, size.y / 2.f);
}

void Grounding::updateLabelScale()
{
    float t = phaseTimer / kGroundingPhaseDuration;
    if (t > 1.f) t = 1.f;

    switch (phase)
    {
    case BreathPhase::Inhale:
        // Simulates lungs filling: label grows from small to large.
        labelScale = kGroundingLabelMinScale + (kGroundingLabelMaxScale - kGroundingLabelMinScale) * t;
        break;
    case BreathPhase::HoldIn:
        labelScale = kGroundingLabelMaxScale; // holds at the grown size
        break;
    case BreathPhase::Exhale:
        // Simulates lungs emptying: label shrinks from large back to small.
        labelScale = kGroundingLabelMaxScale - (kGroundingLabelMaxScale - kGroundingLabelMinScale) * t;
        break;
    case BreathPhase::HoldOut:
        labelScale = kGroundingLabelMinScale; // holds at the shrunk size
        break;
    default:
        break;
    }

    labelSprite.setScale(labelScale, labelScale);
}

void Grounding::update(float deltaTime)
{
    if (finished) return;

   
    bool requiresRelease = (phase == BreathPhase::HoldIn || phase == BreathPhase::HoldOut);
    bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool shouldAdvance = requiresRelease ? !spacePressed : spacePressed;

    if (shouldAdvance)
    {
        phaseTimer += deltaTime;

        if (phaseTimer >= kGroundingPhaseDuration)
        {
            phaseTimer = 0.f;

            switch (phase)
            {
            case BreathPhase::Inhale:  phase = BreathPhase::HoldIn;   break;
            case BreathPhase::HoldIn:  phase = BreathPhase::Exhale;   break;
            case BreathPhase::Exhale:  phase = BreathPhase::HoldOut;  break;
            case BreathPhase::HoldOut: phase = BreathPhase::Finished; finished = true; break;
            default: break;
            }

            setPhaseVisuals();
        }
    }

    updateLabelScale();

    // Map the current phase to which box edge is actively filling.
    float edgeProgress = phaseTimer / kGroundingPhaseDuration;
    if (edgeProgress > 1.f) edgeProgress = 1.f;

    int edgeIndex = 0;
    switch (phase)
    {
    case BreathPhase::Inhale:  edgeIndex = 0; break; // top
    case BreathPhase::HoldIn:  edgeIndex = 1; break; // right
    case BreathPhase::Exhale:  edgeIndex = 2; break; // bottom
    case BreathPhase::HoldOut: edgeIndex = 3; break; // left
    default: edgeIndex = 3; edgeProgress = 1.f; break; // Finished
    }
    boxProgress.setProgress(edgeIndex, edgeProgress);
}

void Grounding::draw(sf::RenderWindow& window)
{
    window.draw(backgroundSprite);
    boxProgress.draw(window);
    window.draw(labelSprite);
}

void Grounding::reset()
{
    phase = BreathPhase::Inhale;
    phaseTimer = 0.f;
    finished = false;
    setPhaseVisuals();
    labelScale = kGroundingLabelMinScale;
    labelSprite.setScale(labelScale, labelScale);
    boxProgress.setProgress(0, 0.f);
}