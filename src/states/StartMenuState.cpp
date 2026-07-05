#include "StartMenuState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

using namespace Constants;

namespace
{
    float frand(float a, float b)
    {
        return a + (b - a) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }

    void fitBackgroundToWindow(sf::Sprite& sprite, const sf::Texture& texture)
    {
        sf::Vector2u size = texture.getSize();
        if (size.x > 0 && size.y > 0)
        {
            sprite.setScale(static_cast<float>(kWindowWidth) / size.x,
                static_cast<float>(kWindowHeight) / size.y);
        }
    }

    // Loading bar (pink pill)
    constexpr float kBarWidth = 400.f;
    constexpr float kBarHeight = 26.f;
    constexpr float kBarX = 640.f - kBarWidth / 2.f;
    constexpr float kBarY = 470.f; 
    const sf::Color kTrackColor(248, 242, 252);
    const sf::Color kFillColor(250, 150, 190);
    constexpr float kLoadDuration = 5.5f;
    constexpr float kFadeOutDuration = 0.8f; 

    //  Buttons
    constexpr float kPlayWidth = 220.f, kPlayX = 640.f, kPlayY = 450.f;
    constexpr float kTutorialWidth = 220.f, kTutorialX = 640.f, kTutorialY = 550.f;
    constexpr float kSettingsWidth = 80.f, kSettingsX = 90.f, kSettingsY = 650.f;
    constexpr float kHoverShrink = 0.9f;

    //  Rising bubbles 
    constexpr int kInitialBubbleCount = 14;
    constexpr int kForcedNearTopCount = 5;  // spawned already near the top so they pop almost immediately
    constexpr float kNearTopYMin = -10.f, kNearTopYMax = 120.f;
    constexpr float kRiseSpawnIntervalMin = 0.12f, kRiseSpawnIntervalMax = 0.30f;
    constexpr float kBubbleDiameterMin = 16.f, kBubbleDiameterMax = 48.f;
    constexpr float kBubbleBaseXMin = 40.f, kBubbleBaseXMax = 1240.f;
    constexpr float kBubbleSpeedMin = 55.f, kBubbleSpeedMax = 150.f;
    constexpr float kBubbleAmplitudeMin = 6.f, kBubbleAmplitudeMax = 22.f;
    constexpr float kBubbleFrequencyMin = 1.2f, kBubbleFrequencyMax = 3.2f;
    constexpr float kBubbleStartYRandomMin = 60.f, kBubbleStartYRandomMax = 720.f;
    constexpr float kBubbleStartYBottomMin = 730.f, kBubbleStartYBottomMax = 820.f;
    constexpr float kBubblePopY = -30.f; // above this, a rising bubble is considered "popped"

    // Audio tuning 
    constexpr std::size_t kPopVoiceCount = 5;
    constexpr float kPopPitchMin = 0.90f, kPopPitchMax = 1.25f;
    constexpr float kPopVolumeMin = 55.f, kPopVolumeMax = 80.f;
   
}

void StartMenuState::init()
{
    // Backgrounds 
    bgIntroTexture = TextureManager::instance().acquire(kStartMenuBgIntroPath);
    bgIntroTexture->setSmooth(true);
    bgIntroSprite.setTexture(*bgIntroTexture);
    fitBackgroundToWindow(bgIntroSprite, *bgIntroTexture);

    bgMenuTexture = TextureManager::instance().acquire(kStartMenuBgMenuPath);
    bgMenuTexture->setSmooth(true);
    bgMenuSprite.setTexture(*bgMenuTexture);
    fitBackgroundToWindow(bgMenuSprite, *bgMenuTexture);

    //  Loading bar: cornerRadius = height / 2 gives the fully rounded "pill" look 
    loadingBar.init(kBarX, kBarY, kBarWidth, kBarHeight, kBarHeight / 2.f, kTrackColor, kFillColor);

    // Buttons
    playTexture = TextureManager::instance().acquire(kStartMenuPlayPath);
    playTexture->setSmooth(true);
    playPressedTexture = TextureManager::instance().acquire(kStartMenuPlayPressedPath);
    playPressedTexture->setSmooth(true);
    playButton.init(playTexture, playPressedTexture, kPlayWidth, kPlayX, kPlayY);

    tutorialTexture = TextureManager::instance().acquire(kStartMenuTutorialPath);
    tutorialTexture->setSmooth(true);
    tutorialPressedTexture = TextureManager::instance().acquire(kStartMenuTutorialPressedPath);
    tutorialPressedTexture->setSmooth(true);
    tutorialButton.init(tutorialTexture, tutorialPressedTexture, kTutorialWidth, kTutorialX, kTutorialY);

    settingsTexture = TextureManager::instance().acquire(kStartMenuSettingsPath);
    settingsTexture->setSmooth(true);
    settingsPressedTexture = TextureManager::instance().acquire(kStartMenuSettingsPressedPath);
    settingsPressedTexture->setSmooth(true);
    settingsButton.init(settingsTexture, settingsPressedTexture, kSettingsWidth, kSettingsX, kSettingsY);

    // Bubble artwork (7 variants)
    for (int i = 0; i < 7; ++i)
    {
        std::string path = std::string(kStartMenuBubblePathPrefix) + std::to_string(i + 1) + ".png";
        bubbleTextures[i] = TextureManager::instance().acquire(path);
        bubbleTextures[i]->setSmooth(true);
    }

    //  Audio 
    popBuffer = SoundManager::instance().acquire(kStartMenuPopSoundPath);
    popSounds.init(popBuffer, kPopVoiceCount);

    clickBuffer = SoundManager::instance().acquire(kUiClickSoundPath);
    clickSound.setBuffer(*clickBuffer);

    // Populate the screen with bubbles up front; force a few near the top so
    // the first pop sound plays almost immediately once loading starts.
    for (int i = 0; i < kInitialBubbleCount; ++i)
    {
        spawnRiseBubble(true);

        if (i < kForcedNearTopCount && !riseBubbles.empty())
        {
            RiseBubble& bubble = riseBubbles.back();
            bubble.sprite.setPosition(bubble.baseX, frand(kNearTopYMin, kNearTopYMax));
        }
    }
}

void StartMenuState::spawnRiseBubble(bool randomHeight)
{
    RiseBubble bubble;
    int index = rand() % 7;
    bubble.sprite.setTexture(*bubbleTextures[index]);

    sf::Vector2u textureSize = bubbleTextures[index]->getSize();
    float referenceSize = (textureSize.x > 0) ? static_cast<float>(textureSize.x) : 32.f;

    float diameter = frand(kBubbleDiameterMin, kBubbleDiameterMax);
    float scale = diameter / referenceSize;
    bubble.sprite.setScale(scale, scale);
    bubble.sprite.setOrigin(referenceSize / 2.f, referenceSize / 2.f);

    bubble.baseX = frand(kBubbleBaseXMin, kBubbleBaseXMax);
    bubble.speed = frand(kBubbleSpeedMin, kBubbleSpeedMax);
    bubble.amplitude = frand(kBubbleAmplitudeMin, kBubbleAmplitudeMax);
    bubble.frequency = frand(kBubbleFrequencyMin, kBubbleFrequencyMax);
    bubble.phaseOffset = frand(0.f, 6.28f);

    float startY = randomHeight
        ? frand(kBubbleStartYRandomMin, kBubbleStartYRandomMax)
        : frand(kBubbleStartYBottomMin, kBubbleStartYBottomMax);
    bubble.sprite.setPosition(bubble.baseX, startY);

    riseBubbles.push_back(bubble);
}

void StartMenuState::playPop()
{
    if (phase != MenuPhase::Loading) return;

   
    float progress = std::clamp(loadTimer / kLoadDuration, 0.f, 1.f);
    float fade = std::max(0.f, 1.f - std::sqrt(progress));
    if (fade <= 0.02f) return; // effectively silent, skip playing entirely

    popSounds.play(frand(kPopPitchMin, kPopPitchMax), frand(kPopVolumeMin, kPopVolumeMax) * fade);
}

void StartMenuState::playClickSound()
{
    if (clickBuffer && clickBuffer->getSampleCount() > 0)
    {
        clickSound.setPitch(kUiClickPitch);
        clickSound.setVolume(kUiClickVolume);
        clickSound.play();
    }
}

void StartMenuState::updateLoading(float deltaTime)
{
    elapsed += deltaTime;

    riseSpawnTimer += deltaTime;
    if (riseSpawnTimer >= nextRiseSpawn)
    {
        riseSpawnTimer = 0.f;
        nextRiseSpawn = frand(kRiseSpawnIntervalMin, kRiseSpawnIntervalMax);
        spawnRiseBubble(false);
    }

  
    for (int i = static_cast<int>(riseBubbles.size()) - 1; i >= 0; --i)
    {
        RiseBubble& bubble = riseBubbles[i];
        float y = bubble.sprite.getPosition().y - bubble.speed * deltaTime;
        float x = bubble.baseX + std::sin(elapsed * bubble.frequency + bubble.phaseOffset) * bubble.amplitude;
        bubble.sprite.setPosition(x, y);

        if (y < kBubblePopY)
        {
            playPop();
            riseBubbles.erase(riseBubbles.begin() + i);
        }
    }

    loadTimer += deltaTime;
    loadingBar.setProgress(loadTimer / kLoadDuration);

    if (loadTimer >= kLoadDuration)
    {
        popSounds.beginFadeOut(kFadeOutDuration);
        phase = MenuPhase::Ready;
    }
}

void StartMenuState::updateReady(const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    playButton.update(mousePos, kHoverShrink);
    tutorialButton.update(mousePos, kHoverShrink);
    settingsButton.update(mousePos, kHoverShrink);
}

void StartMenuState::update(float deltaTime, sf::RenderWindow& window)
{
    if (phase == MenuPhase::Ready)
    {
        updateReady(window);
        popSounds.update(deltaTime); 
        return;
    }

    updateLoading(deltaTime);
}

void StartMenuState::render(sf::RenderWindow& window)
{
    if (phase == MenuPhase::Ready)
    {
        window.draw(bgMenuSprite);
        playButton.draw(window);
        tutorialButton.draw(window);
        settingsButton.draw(window);
        return;
    }

    window.draw(bgIntroSprite);

    for (auto& bubble : riseBubbles)
    {
        window.draw(bubble.sprite);
    }

    loadingBar.draw(window);
}

void StartMenuState::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        handleMousePress(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        handleClick(mousePos); 
        handleMouseRelease();
    }
}

void StartMenuState::handleMousePress(const sf::Vector2f& mousePos)
{
    if (phase != MenuPhase::Ready) return;

    pressedButton = -1;
    if (playButton.contains(mousePos))           pressedButton = 0;
    else if (tutorialButton.contains(mousePos))  pressedButton = 1;
    else if (settingsButton.contains(mousePos))  pressedButton = 2;

    
    if (pressedButton != -1)
    {
        playClickSound();
    }
}

void StartMenuState::handleMouseRelease()
{
    pressedButton = -1;
}

void StartMenuState::handleClick(const sf::Vector2f& mousePos)
{
    if (phase != MenuPhase::Ready) return;

   
    if (pressedButton == 0 && playButton.contains(mousePos))
        playClicked = true;
    else if (pressedButton == 1 && tutorialButton.contains(mousePos))
        tutorialClicked = true;
    else if (pressedButton == 2 && settingsButton.contains(mousePos))
        settingsClicked = true;
}

bool StartMenuState::wasPlayClicked()
{
    if (playClicked) { playClicked = false; return true; }
    return false;
}

bool StartMenuState::wasTutorialClicked()
{
    if (tutorialClicked) { tutorialClicked = false; return true; }
    return false;
}

bool StartMenuState::wasSettingsClicked()
{
    if (settingsClicked) { settingsClicked = false; return true; }
    return false;
}