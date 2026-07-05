#include "GuideState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"

using namespace Constants;

void GuideState::init()
{
    // Menu art behind the popup (cached; StartMenuState already loaded it) ---
    menuBackgroundTexture = TextureManager::instance().acquire(kStartMenuBgMenuPath);
    menuBackgroundTexture->setSmooth(true);
    menuBackgroundSprite.setTexture(*menuBackgroundTexture);

    sf::Vector2u menuSize = menuBackgroundTexture->getSize();
    if (menuSize.x > 0 && menuSize.y > 0)
    {
        menuBackgroundSprite.setScale(
            static_cast<float>(kWindowWidth) / menuSize.x,
            static_cast<float>(kWindowHeight) / menuSize.y);
    }

    dimOverlay.setSize(sf::Vector2f(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)));
    dimOverlay.setFillColor(kGuideDimColor);
    dimOverlay.setPosition(0.f, 0.f);

    // Guide image, centered 
    guideTexture = TextureManager::instance().acquire(kGuidePopupTexturePath);
    guideTexture->setSmooth(true);
    guideSprite.setTexture(*guideTexture);

    sf::Vector2u guideSize = guideTexture->getSize();
    float guideScale = (guideSize.x > 0) ? kGuidePopupTargetWidth / static_cast<float>(guideSize.x) : 1.f;
    guideSprite.setScale(guideScale, guideScale);
    guideSprite.setOrigin(guideSize.x / 2.f, guideSize.y / 2.f);
    guideSprite.setPosition(kGuidePopupPos);

    closeTexture = TextureManager::instance().acquire(kCloseButtonTexturePath);
    closeTexture->setSmooth(true);
    closePressedTexture = TextureManager::instance().acquire(kCloseButtonPressedTexturePath);
    closePressedTexture->setSmooth(true);

    sf::FloatRect bounds = guideSprite.getGlobalBounds();
    sf::Vector2f topRight(bounds.left + bounds.width, bounds.top);
    closeButton.init(closeTexture, closePressedTexture, kCloseButtonTargetWidth, topRight, kCloseButtonInset);

    clickBuffer = SoundManager::instance().acquire(kUiClickSoundPath);
    clickSound.setBuffer(*clickBuffer);
}

void GuideState::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    closeButton.handleEvent(event, window);
}

void GuideState::update(float deltaTime, sf::RenderWindow& window)
{
    (void)deltaTime; 

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    closeButton.update(mousePos);

    if (closeButton.wasClicked())
    {
        playClickSound();
        closedFlag = true;
    }
}

void GuideState::render(sf::RenderWindow& window)
{
    window.draw(menuBackgroundSprite);
    window.draw(dimOverlay);
    window.draw(guideSprite);
    closeButton.draw(window);
}

void GuideState::playClickSound()
{
    if (clickBuffer && clickBuffer->getSampleCount() > 0)
    {
        clickSound.setPitch(kUiClickPitch);
        clickSound.setVolume(kUiClickVolume);
        clickSound.play();
    }
}

bool GuideState::wasClosed()
{
    if (closedFlag)
    {
        closedFlag = false;
        return true;
    }
    return false;
}