#include "Distraction.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"
#include "../ui/TextHelper.h"
#include <cstdlib>

using namespace Constants;

namespace
{
    // Popup box visual growth 
    constexpr float kBaseBoxWidth = 260.f;
    constexpr float kBaseBoxHeight = 90.f;
    constexpr float kBoxGrowthPerPopup = 30.f;
    constexpr float kSpawnAreaWidth = 1280.f;
    constexpr float kSpawnAreaHeight = 600.f; // intentionally less than the full window height,
    // leaving room near the bottom for the Grounding button
    constexpr float kSpawnMargin = 20.f;

    // Popup text auto fit
    constexpr float kTextPaddingXRatio = 0.12f;
    constexpr float kTextPaddingYRatio = 0.15f;
    constexpr float kTextVerticalBiasRatio = 0.08f;
    constexpr float kBaseFontSize = 14.f;
    constexpr float kFontGrowthPerPopup = 0.5f;
    constexpr unsigned int kMaxFontSize = 48;
    constexpr unsigned int kMinFontSize = 8;

    // Spawn interval curve: gets faster as popupCount rises 
    constexpr int kRampUpPopupCount = 4; 
    constexpr float kEarlySpawnBase = 1.4f;
    constexpr float kMidSpawnBase = 1.4f;
    constexpr float kMidSpawnStep = 0.2f;
    constexpr float kChaosSpawnInterval = 0.18f;

    // Grounding button 
    // Centered where the old shape button used to sit; width is tunable.
    constexpr float kGroundingButtonWidth = 300.f;
    constexpr float kGroundingButtonCenterX = 640.f;
    constexpr float kGroundingButtonCenterY = 620.f;
}

Distraction::Distraction()
{
    messages = {
        "Did I leave the stove on?",
        "What was I doing again?",
        "I should text them back.",
        "New notification!",
        "Wait, what time is it?",
        "I forgot something.",
        "I should read more",
        "Did you see that?"
    };
}

void Distraction::init(sf::Font& fontRef)
{
    font = &fontRef;

    popupTexture = TextureManager::instance().acquire(kDistractionPopupTexturePath);

    nextSpawn = kDistractionStartDelay + static_cast<float>(rand() % 3);

    groundingNormalTexture = TextureManager::instance().acquire(kGroundingButtonNormalTexturePath);
    groundingNormalTexture->setSmooth(true);
    groundingPressedTexture = TextureManager::instance().acquire(kGroundingButtonPressedTexturePath);
    groundingPressedTexture->setSmooth(true);
    groundingButton.init(groundingNormalTexture, groundingPressedTexture,
        kGroundingButtonWidth, kGroundingButtonCenterX, kGroundingButtonCenterY);


    groundingClickBuffer = SoundManager::instance().acquire(kUiClickSoundPath);
    groundingClickSound.setBuffer(*groundingClickBuffer);
}

void Distraction::spawnPopup()
{
    PopupBox popup;


    float sizeGrowth = popupCount * kBoxGrowthPerPopup;
    float boxW = kBaseBoxWidth + sizeGrowth;
    float boxH = kBaseBoxHeight + sizeGrowth;

    popup.box.setTexture(*popupTexture);

    float scaleX = boxW / static_cast<float>(popupTexture->getSize().x);
    float scaleY = boxH / static_cast<float>(popupTexture->getSize().y);
    popup.box.setScale(scaleX, scaleY);

    float maxX = kSpawnAreaWidth - boxW - kSpawnMargin;
    float maxY = kSpawnAreaHeight - boxH - kSpawnMargin;
    if (maxX < kSpawnMargin) maxX = kSpawnMargin;
    if (maxY < kSpawnMargin) maxY = kSpawnMargin;

    float x = kSpawnMargin + static_cast<float>(rand() % static_cast<int>(maxX));
    float y = kSpawnMargin + static_cast<float>(rand() % static_cast<int>(maxY));
    popup.box.setPosition(x, y);

    popup.text.setFont(*font);
    popup.text.setFillColor(sf::Color::Black);
    popup.text.setString(messages[rand() % messages.size()]);

    float paddingX = boxW * kTextPaddingXRatio;
    float paddingY = boxH * kTextPaddingYRatio;
    float maxTextWidth = boxW - (paddingX * 2.f);
    float maxTextHeight = boxH - (paddingY * 2.f);

    unsigned int fontSize = static_cast<unsigned int>(kBaseFontSize + sizeGrowth * kFontGrowthPerPopup);
    if (fontSize > kMaxFontSize) fontSize = kMaxFontSize;
    popup.text.setCharacterSize(fontSize);

    TextHelper::fitToBox(popup.text, maxTextWidth, maxTextHeight, kMinFontSize);

    float centerX = x + boxW / 2.f;
    float centerY = y + boxH / 2.f - (boxH * kTextVerticalBiasRatio);
    TextHelper::centerAt(popup.text, centerX, centerY);

    popups.push_back(popup);
    popupCount++;

    if (popupCount >= kDistractionChaosThreshold)
    {
        chaosMode = true;
    }

    if (popupCount >= kDistractionMaxPopups)
    {
        groundingButtonVisible = true;
    }
}

void Distraction::update(float deltaTime)
{
    if (groundingButtonVisible)
    {
        return;
    }

    spawnTimer += deltaTime;

    if (spawnTimer >= nextSpawn)
    {
        spawnTimer = 0.f;
        spawnPopup();

        if (popupCount < kRampUpPopupCount)
        {
            nextSpawn = kEarlySpawnBase + static_cast<float>(rand() % 2);
        }
        else if (popupCount < kDistractionChaosThreshold)
        {
            nextSpawn = kMidSpawnBase + static_cast<float>(rand() % 2) * kMidSpawnStep;
        }
        else
        {
            nextSpawn = kChaosSpawnInterval;
        }
    }
}

void Distraction::draw(sf::RenderWindow& window)
{
    for (auto& popup : popups)
    {
        window.draw(popup.box);
        window.draw(popup.text);
    }

    if (groundingButtonVisible)
    {
       
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        groundingButton.update(mousePos);
        groundingButton.draw(window);
    }
}

void Distraction::handleClick(sf::Vector2f mousePos)
{
    if (groundingButtonVisible && groundingButton.contains(mousePos))
    {
        groundingClickSound.play();
        groundingRequested = true;
    }
}

bool Distraction::wasGroundingRequested()
{
    if (groundingRequested)
    {
        groundingRequested = false;
        return true;
    }
    return false;
}

void Distraction::reset()
{
    popups.clear();
    popupCount = 0;
    chaosMode = false;
    groundingButtonVisible = false;
    groundingRequested = false;
    spawnTimer = 0.f;

   
    nextSpawn = kDistractionStartDelay + static_cast<float>(rand() % 15);
}