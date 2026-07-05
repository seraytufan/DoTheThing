#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "../core/GameState.h"
#include "../entities/Player.h"
#include "../mechanics/Hyperfocus.h"
#include "../mechanics/Distraction.h"
#include "../world/LaundryRoomBackground.h"
#include "../world/ObjectSpawner.h"
#include "../ui/CountdownTimer.h"
#include "../ui/HeartDisplay.h"
#include "../ui/Button.h"
#include "../ui/SpritesheetPopEffect.h"
#include "../ui/CoinBurstEffect.h"
#include "../audio/SoundPool.h"
#include "TodoPopupState.h"


class PlayingState : public GameState
{
public:
    void init(sf::Font& fontRef);

    void onEnter() override;
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

   
    bool wasGroundingRequested();

 
    bool wasReturnToMenuRequested();

private:
    void updateWorldScrolling(float deltaTime);
    void handleDirtyLaundryCollisions();
    void handleRockCollisions();
    void updateScoreText();

    enum class OutcomePhase { None, WinAnimating, WinPopup, LosePopup };

    void beginWinSequence();
    void beginLoseSequence();
    void updateOutcome(float deltaTime, sf::RenderWindow& window);
    void handleOutcomeClick(const sf::Vector2f& mousePos);
    void resetForRetry();   
    void resetForNewGame(); 

    Player player;
    Hyperfocus hyperfocus;
    Distraction distraction;

    LaundryRoomBackground background;
    ObjectSpawner dirtyLaundrySpawner;
    ObjectSpawner rockSpawner;

    std::shared_ptr<sf::Texture> coinTexture; 
    sf::Sprite coinSprite;

    
    CoinBurstEffect coinBurst;
    std::shared_ptr<sf::SoundBuffer> coinSoundBuffer;
    SoundPool coinSound;

    std::shared_ptr<sf::SoundBuffer> errorSoundBuffer;
    SoundPool errorSound;

    sf::Text scoreText;
    int score = 0;

    bool groundingRequestedFlag = false;

    TodoPopupState todoPopup;
    bool hasShownIntroPopup = false;
    bool showingIntroPopup = false;

 
    CountdownTimer countdown;
    HeartDisplay hearts;
    int lives = 0; 


    OutcomePhase outcomePhase = OutcomePhase::None;
    bool returnToMenuRequestedFlag = false;

    SpritesheetPopEffect winAnimation;

    std::shared_ptr<sf::Texture> winPanelTexture, losePanelTexture;
    sf::Sprite outcomePanelSprite; 

    std::shared_ptr<sf::Texture> menuTexture, menuPressedTexture;
    std::shared_ptr<sf::Texture> replayTexture, replayPressedTexture;
    Button menuButton;
    Button replayButton;
};