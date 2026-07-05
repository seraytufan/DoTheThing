#include "PlayingState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"
#include <cmath>
#include <cstdlib>
#include <string>

using namespace Constants;

namespace
{
    // Dirty laundry (collectible) 
    constexpr float kLaundryScale = 0.45f;
    const sf::Vector2f kLaundrySpawnPos(1280.f, 450.f);
    constexpr float kLaundryInitialSpawnBase = 0.f;
    constexpr int kLaundryInitialSpawnRandRange = 2;    
    constexpr float kLaundryInitialSpawnStep = 0.1f;
    constexpr float kLaundryRespawnBase = 1.7f;
    constexpr int kLaundryRespawnRandRange = 2;

    // Rocks / water puddles (hazard) 
    constexpr float kRockScale = 0.40f;
    const sf::Vector2f kRockSpawnPos(1280.f, 530.f);
    constexpr float kRockInitialSpawnBase = 0.7f;
    constexpr int kRockInitialSpawnRandRange = 2;
    constexpr float kRockInitialSpawnStep = 0.1f;
    constexpr float kRockRespawnBase = 2.f;
    constexpr int kRockRespawnRandRange = 3;

    float randLaundryRespawn()
    {
        return kLaundryRespawnBase + static_cast<float>(rand() % kLaundryRespawnRandRange);
    }

    float randRockRespawn()
    {
        return kRockRespawnBase + static_cast<float>(rand() % kRockRespawnRandRange);
    }
}

void PlayingState::init(sf::Font& fontRef)
{
    hyperfocus.init(fontRef);
    distraction.init(fontRef);
    background.init();

    auto laundryTexture = TextureManager::instance().acquire(kDirtyLaundryTexturePath);
    float laundryInitial = kLaundryInitialSpawnBase +
        static_cast<float>(rand() % kLaundryInitialSpawnRandRange) * kLaundryInitialSpawnStep;
    dirtyLaundrySpawner.init(laundryTexture, kLaundryScale, kLaundrySpawnPos, laundryInitial, randLaundryRespawn);

    auto rockTexture = TextureManager::instance().acquire(kPuddleTexturePath);
    float rockInitial = kRockInitialSpawnBase +
        static_cast<float>(rand() % kRockInitialSpawnRandRange) * kRockInitialSpawnStep;
    rockSpawner.init(rockTexture, kRockScale, kRockSpawnPos, rockInitial, randRockRespawn);

    coinTexture = TextureManager::instance().acquire(kCoinTexturePath);
    coinSprite.setTexture(*coinTexture);
    coinSprite.setScale(kCoinScale, kCoinScale);
    coinSprite.setPosition(kCoinPos);

    coinBurst.init(kCoinBurstTexturePath, kCoinBurstGridCols, kCoinBurstGridRows,
        kCoinBurstFrameTime, kCoinBurstScale);

    coinSoundBuffer = SoundManager::instance().acquire(kCoinSoundPath);
    coinSound.init(coinSoundBuffer, kCoinSoundVoiceCount);

    errorSoundBuffer = SoundManager::instance().acquire(kErrorSoundPath);
    errorSound.init(errorSoundBuffer, kErrorSoundVoiceCount);

    scoreText.setFont(fontRef);
    scoreText.setCharacterSize(kScoreTextSize);
    scoreText.setFillColor(kScoreTextColor);
    scoreText.setPosition(kScoreTextPos);
    updateScoreText();

    todoPopup.init();

    countdown.init(fontRef, kCountdownTextSize, kCountdownTextColor, kCountdownDurationSeconds);
    countdown.setPosition(kCountdownPos.x, kCountdownPos.y);

    lives = kStartingLives;
    hearts.init(kStartingLives, kHeartsPos, kHeartTargetWidth, kHeartSpacing, kHeartFullTexturePath, kHeartLostTexturePath);

    winAnimation.init(kWinAnimationTexturePath, kWinAnimationGridCols, kWinAnimationGridRows,
        kWinAnimationFrameTime, kWinAnimationGrowDuration, kWinAnimationHoldDuration, kWinAnimationShrinkDuration,
        kWinAnimationScale, kWinAnimationPos);

    // Outcome panel image (win / lose textures share one sprite) 
    winPanelTexture = TextureManager::instance().acquire(kWinPanelTexturePath);
    winPanelTexture->setSmooth(true);
    losePanelTexture = TextureManager::instance().acquire(kLosePanelTexturePath);
    losePanelTexture->setSmooth(true);

    sf::Vector2u panelSize = winPanelTexture->getSize();
    float panelScale = (panelSize.x > 0) ? kOutcomePanelTargetWidth / static_cast<float>(panelSize.x) : 1.f;
    outcomePanelSprite.setScale(panelScale, panelScale);
    outcomePanelSprite.setOrigin(panelSize.x / 2.f, panelSize.y / 2.f);
    outcomePanelSprite.setPosition(kPopupPanelPos);
    outcomePanelSprite.setTexture(*winPanelTexture, true); 

    //  Menu / Replay image buttons (same on both win and lose) 
    menuTexture = TextureManager::instance().acquire(kOutcomeMenuTexturePath);
    menuTexture->setSmooth(true);
    menuPressedTexture = TextureManager::instance().acquire(kOutcomeMenuPressedTexturePath);
    menuPressedTexture->setSmooth(true);
    menuButton.init(menuTexture, menuPressedTexture, kOutcomeButtonWidth,
        kPopupPanelPos.x - kOutcomeButtonXSpacing, kOutcomeButtonY);

    replayTexture = TextureManager::instance().acquire(kOutcomeReplayTexturePath);
    replayTexture->setSmooth(true);
    replayPressedTexture = TextureManager::instance().acquire(kOutcomeReplayPressedTexturePath);
    replayPressedTexture->setSmooth(true);
    replayButton.init(replayTexture, replayPressedTexture, kOutcomeButtonWidth,
        kPopupPanelPos.x + kOutcomeButtonXSpacing, kOutcomeButtonY);
}

void PlayingState::onEnter()
{
    // Only the very first entrance (right after Story) shows the intro popup.
    if (!hasShownIntroPopup)
    {
        hasShownIntroPopup = true;
        showingIntroPopup = true;
        todoPopup.onEnter();
    }
}

void PlayingState::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (showingIntroPopup) return; // ignore all input while the intro popup is up

    if (outcomePhase == OutcomePhase::WinAnimating)
    {
        return; // no input during the celebration animation
    }

    if (outcomePhase == OutcomePhase::WinPopup || outcomePhase == OutcomePhase::LosePopup)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            handleOutcomeClick(mousePos);
        }
        return;
    }

    // Grounding button reacts on press (matches the original).
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        distraction.handleClick(mousePos);
    }
}

void PlayingState::update(float deltaTime, sf::RenderWindow& window)
{
    (void)window;

    if (showingIntroPopup)
    {
        todoPopup.update(deltaTime, window);
        if (todoPopup.wasFinished())
        {
            showingIntroPopup = false;
        }
        return;
    }

    if (outcomePhase != OutcomePhase::None)
    {
        updateOutcome(deltaTime, window);
        return;
    }

    if (!hyperfocus.isActive())
    {
        distraction.update(deltaTime);
    }

    if (!hyperfocus.isActive() && distraction.wasGroundingRequested())
    {
        groundingRequestedFlag = true;
        distraction.reset();
        return;
    }

    if (distraction.isGroundingButtonVisible())
    {
        updateScoreText();
        return;
    }

    bool hyperfocusWasActive = hyperfocus.isActive();

    updateWorldScrolling(deltaTime);
    handleDirtyLaundryCollisions();
    handleRockCollisions();

    hyperfocus.update(deltaTime, score);

    if (!hyperfocusWasActive && hyperfocus.isActive())
    {
        player.forceGrounded();
    }

    if (!hyperfocus.isActive())
    {
        player.update(deltaTime);
    }

    bool moving = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || hyperfocus.isActive();
    float animSpeedMultiplier = hyperfocus.isActive() ? kHyperfocusSpeedMultiplier : 1.f;
    player.updateAnimation(deltaTime, moving, animSpeedMultiplier);

    countdown.update(deltaTime, true);
    coinBurst.update(deltaTime);

    updateScoreText();

    // Check win/lose AFTER this frame's score/timer updates.
    if (score >= kWinScoreThreshold)
    {
        beginWinSequence();
    }
    else if (countdown.isExpired())
    {
        beginLoseSequence();
    }
}

void PlayingState::beginWinSequence()
{
    outcomePhase = OutcomePhase::WinAnimating;
    winAnimation.start();
}

void PlayingState::beginLoseSequence()
{
    outcomePhase = OutcomePhase::LosePopup;
    outcomePanelSprite.setTexture(*losePanelTexture, true);
}

void PlayingState::updateOutcome(float deltaTime, sf::RenderWindow& window)
{
    if (outcomePhase == OutcomePhase::WinAnimating)
    {
        winAnimation.update(deltaTime);
        if (winAnimation.isFinished())
        {
            outcomePhase = OutcomePhase::WinPopup;
            outcomePanelSprite.setTexture(*winPanelTexture, true);
        }
        return;
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    menuButton.update(mousePos, 0.9f);

    // Replay hover: always on win; on lose only while hearts remain.
    if (outcomePhase == OutcomePhase::WinPopup || lives > 0)
    {
        replayButton.update(mousePos, 0.9f);
    }
}

void PlayingState::handleOutcomeClick(const sf::Vector2f& mousePos)
{
    if (outcomePhase == OutcomePhase::WinPopup)
    {
        if (menuButton.contains(mousePos))
        {
            resetForNewGame();
            returnToMenuRequestedFlag = true;
        }
        else if (replayButton.contains(mousePos))
        {
            resetForNewGame(); // win replay: full fresh start, hearts refilled
        }
    }
    else if (outcomePhase == OutcomePhase::LosePopup)
    {
        if (menuButton.contains(mousePos))
        {
            resetForNewGame();
            returnToMenuRequestedFlag = true;
        }
        else if (lives > 0 && replayButton.contains(mousePos))
        {
        
            lives--;
            hearts.setLivesRemaining(lives);
            resetForRetry();
        }
    }
}

void PlayingState::resetForNewGame()
{
    score = 0;
    lives = kStartingLives;
    hearts.setLivesRemaining(lives);
    countdown.reset(kCountdownDurationSeconds);
    hyperfocus.reset();
    distraction.reset();
    player.reset(); 
    outcomePhase = OutcomePhase::None;
    updateScoreText();
   
}

void PlayingState::resetForRetry()
{
    score = 0;
    countdown.reset(kCountdownDurationSeconds);
    hyperfocus.reset();
    distraction.reset();
    player.reset(); 
    outcomePhase = OutcomePhase::None;
    updateScoreText();
    
}

void PlayingState::updateWorldScrolling(float deltaTime)
{
    bool worldMoving = hyperfocus.isActive() || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    float speed = hyperfocus.isActive() ? kWorldSpeed * kHyperfocusSpeedMultiplier : kWorldSpeed;
    float pixels = worldMoving ? speed * deltaTime : 0.f;

    background.update(deltaTime, pixels, worldMoving);
    dirtyLaundrySpawner.update(deltaTime, pixels, worldMoving, kOffscreenRemovalX);
    rockSpawner.update(deltaTime, pixels, worldMoving, kOffscreenRemovalX);
}

void PlayingState::handleDirtyLaundryCollisions()
{
    auto& items = dirtyLaundrySpawner.items();

    for (int i = static_cast<int>(items.size()) - 1; i >= 0; --i)
    {
        bool collected = items[i].getGlobalBounds().intersects(player.getGlobalBounds());

        if (!collected && hyperfocus.isActive())
        {
            float distance = std::abs(items[i].getPosition().x - player.getPosition().x);
            if (distance < kAutoCollectRadius)
            {
                collected = true;
            }
        }

        if (collected)
        {
            int gain = hyperfocus.isActive() ? kLaundryGainHyperfocus : kLaundryGainNormal;
            hyperfocus.tryActivate(score, gain);

            if (hyperfocus.isActive())
            {
                distraction.reset();
            }

            sf::Vector2f burstPos = items[i].getPosition();
            burstPos.y += kCoinBurstYOffset;
            coinBurst.trigger(burstPos);
            coinSound.play(kCoinSoundPitch, kCoinSoundVolume);
            player.triggerPickup();

            dirtyLaundrySpawner.removeAt(static_cast<std::size_t>(i));
        }
    }
}

void PlayingState::handleRockCollisions()
{
    auto& items = rockSpawner.items();

    for (int i = static_cast<int>(items.size()) - 1; i >= 0; --i)
    {
        if (items[i].getGlobalBounds().intersects(player.getGlobalBounds()))
        {
            if (!hyperfocus.isActive())
            {
                score -= kRockPenalty;
                if (score < 0) score = 0;

                errorSound.play(kErrorSoundPitch, kErrorSoundVolume);
            }

            rockSpawner.removeAt(static_cast<std::size_t>(i));
        }
    }
}

void PlayingState::updateScoreText()
{
    scoreText.setString("Score = " + std::to_string(score));
}

void PlayingState::render(sf::RenderWindow& window)
{
    background.draw(window);
    dirtyLaundrySpawner.draw(window);
    rockSpawner.draw(window);
    window.draw(coinSprite);
    window.draw(scoreText);
    hyperfocus.draw(window);
    player.draw(window);
    distraction.draw(window);
    coinBurst.draw(window);

    if (!showingIntroPopup)
    {
        countdown.draw(window);
        hearts.draw(window);
    }

    if (showingIntroPopup)
    {
        todoPopup.render(window);
    }

    if (outcomePhase == OutcomePhase::WinAnimating)
    {
        winAnimation.draw(window);
    }
    else if (outcomePhase == OutcomePhase::WinPopup || outcomePhase == OutcomePhase::LosePopup)
    {
        window.draw(outcomePanelSprite);
        menuButton.draw(window);

      
        if (outcomePhase == OutcomePhase::WinPopup || lives > 0)
        {
            replayButton.draw(window);
        }
    }
}

bool PlayingState::wasGroundingRequested()
{
    if (groundingRequestedFlag)
    {
        groundingRequestedFlag = false;
        return true;
    }
    return false;
}

bool PlayingState::wasReturnToMenuRequested()
{
    if (returnToMenuRequestedFlag)
    {
        returnToMenuRequestedFlag = false;
        return true;
    }
    return false;
}