#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Player
{
public:
    Player();


    void update(float deltaTime);

   
    void updateAnimation(float deltaTime, bool moving, float speedMultiplier = 1.f);

    void jump();

    void reset();

    void triggerPickup();
    bool isPicking() const { return picking; }

    void draw(sf::RenderWindow& window);


    void forceGrounded();

    
    void setAutoMode(bool enabled);
    void updateAuto(float deltaTime, const std::vector<sf::RectangleShape>& rocks);

    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;

private:
    void updateSpritePosition();
    void applyTexture(const sf::Texture& texture);

    sf::RectangleShape hitbox;

    
    std::vector<std::shared_ptr<sf::Texture>> runFrames;
    std::vector<std::shared_ptr<sf::Texture>> jumpFrames; // played only while airborne
    std::shared_ptr<sf::Texture> idleTexture; // dedicated idle pose, separate from the run cycle
    std::vector<std::shared_ptr<sf::Texture>> pickingFrames; // played once through on triggerPickup()
    sf::Sprite sprite;
    bool texturesLoaded = false;

    float velocityY = 0.f;
    bool onGround = false;
    bool autoMode = false;

    int currentFrame = 0;
    float animTimer = 0.f;
    bool isMoving = false;

    int jumpFrame = 0; // computed from jump height while airborne, then time while landing-settling
    bool wasAirborne = false;
    bool isLandingSettle = false; // true for a brief window right after touching ground
    float landingSettleTimer = 0.f;

 
    bool picking = false;
    int pickingFrame = 0;
    float pickingTimer = 0.f;
    bool pickingHolding = false; // true while frozen on the last frame, before returning to run/idle
    float pickingHoldTimer = 0.f;
};