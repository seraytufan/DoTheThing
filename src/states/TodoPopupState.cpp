#include "TodoPopupState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"

using namespace Constants;

void TodoPopupState::init()
{
    texture = TextureManager::instance().acquire(kTodoPopupTexturePath);
    texture->setSmooth(true);
    sprite.setTexture(*texture);

    sf::Vector2u size = texture->getSize();
    baseScale = (size.x > 0) ? kTodoPopupTargetWidth / static_cast<float>(size.x) : 1.f;

    sprite.setOrigin(size.x / 2.f, size.y / 2.f);
    sprite.setPosition(kTodoPopupPos);
}

void TodoPopupState::onEnter()
{
    
    phase = Phase::Showing;
    timer = 0.f;
    finishedFlag = false;
    sprite.setScale(baseScale, baseScale);
    sprite.setColor(sf::Color(255, 255, 255, 255));
}

void TodoPopupState::update(float deltaTime, sf::RenderWindow& window)
{
    (void)window;

    timer += deltaTime;

    if (phase == Phase::Showing)
    {
        if (timer >= kTodoPopupShowDuration)
        {
            phase = Phase::Disappearing;
            timer = 0.f;
        }
        return;
    }

   
    float t = timer / kTodoPopupDisappearDuration;
    if (t > 1.f) t = 1.f;

    auto alpha = static_cast<sf::Uint8>(255.f * (1.f - t));
    sprite.setColor(sf::Color(255, 255, 255, alpha));

    if (t >= 1.f)
    {
        finishedFlag = true;
    }
}

void TodoPopupState::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool TodoPopupState::wasFinished()
{
    if (finishedFlag)
    {
        finishedFlag = false;
        return true;
    }
    return false;
}