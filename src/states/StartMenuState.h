#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>

#include "../core/GameState.h"
#include "../ui/Button.h"
#include "../ui/ProgressBar.h"
#include "../audio/SoundPool.h"


enum class MenuPhase
{
    Loading,
    Ready
};


struct RiseBubble
{
    sf::Sprite sprite;
    float speed = 0.f;       // upward speed, pixels/second
    float baseX = 0.f;       // horizontal oscillation center
    float amplitude = 0.f;   // horizontal sway distance
    float frequency = 0.f;   // horizontal sway speed
    float phaseOffset = 0.f; // sway starting offset
};

class StartMenuState : public GameState
{
public:
   
    void init();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

    bool wasPlayClicked();
    bool wasTutorialClicked();
    bool wasSettingsClicked();

    bool isReady() const { return phase == MenuPhase::Ready; }

private:
    void spawnRiseBubble(bool randomHeight);
    void playPop();
    void playClickSound();

    void updateLoading(float deltaTime);
    void updateReady(const sf::RenderWindow& window);

    void handleMousePress(const sf::Vector2f& mousePos);
    void handleMouseRelease();
    void handleClick(const sf::Vector2f& mousePos);

    //  Backgrounds 
    std::shared_ptr<sf::Texture> bgIntroTexture; // logo + loading art
    sf::Sprite bgIntroSprite;
    std::shared_ptr<sf::Texture> bgMenuTexture;  // logo only
    sf::Sprite bgMenuSprite;

    //  Buttons 
    std::shared_ptr<sf::Texture> playTexture, playPressedTexture;
    std::shared_ptr<sf::Texture> tutorialTexture, tutorialPressedTexture;
    std::shared_ptr<sf::Texture> settingsTexture, settingsPressedTexture;
    Button playButton, tutorialButton, settingsButton;

    //  Rising bubbles 
    std::shared_ptr<sf::Texture> bubbleTextures[7];
    std::vector<RiseBubble> riseBubbles;
    float riseSpawnTimer = 0.f;
    float nextRiseSpawn = 0.2f;
    float elapsed = 0.f;

    //  Loading bar 
    ProgressBar loadingBar;
    float loadTimer = 0.f;

    //  Audio 
    std::shared_ptr<sf::SoundBuffer> popBuffer;
    SoundPool popSounds;
    std::shared_ptr<sf::SoundBuffer> clickBuffer;
    sf::Sound clickSound;

    MenuPhase phase = MenuPhase::Loading;

    bool playClicked = false;
    bool tutorialClicked = false;
    bool settingsClicked = false;

   
    int pressedButton = -1;
};