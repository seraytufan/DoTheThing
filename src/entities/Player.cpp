#include "Player.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"
#include <iomanip>
#include <sstream>

using namespace Constants;

namespace
{
    
    std::string framePath(const char* prefix, int frameIndexZeroBased)
    {
        std::ostringstream oss;
        oss << prefix
            << std::setfill('0') << std::setw(kPlayerFrameNumberDigits) << (frameIndexZeroBased + 1)
            << ".png";
        return oss.str();
    }
}

Player::Player()
{
    // Collision hitbox: physics (gravity, jump, ground clamp) is driven by
    // this rectangle; the sprite is purely visual and follows it.
    hitbox.setSize(sf::Vector2f(kPlayerHitboxWidth, kPlayerHitboxHeight));
    hitbox.setPosition(kPlayerStartX, kPlayerStartY);

    runFrames.reserve(kPlayerFrameCount);
    for (int i = 0; i < kPlayerFrameCount; ++i)
    {
        auto tex = TextureManager::instance().acquire(framePath(kPlayerFramePathPrefix, i));
        tex->setSmooth(true);
        runFrames.push_back(tex);
    }

    jumpFrames.reserve(kPlayerJumpFrameCount);
    for (int i = 0; i < kPlayerJumpFrameCount; ++i)
    {
        auto tex = TextureManager::instance().acquire(framePath(kPlayerJumpFramePathPrefix, i));
        tex->setSmooth(true);
        jumpFrames.push_back(tex);
    }

    idleTexture = TextureManager::instance().acquire(kPlayerIdleTexturePath);
    idleTexture->setSmooth(true);

    pickingFrames.reserve(kPlayerPickingFrameCount);
    for (int i = 0; i < kPlayerPickingFrameCount; ++i)
    {
        auto tex = TextureManager::instance().acquire(framePath(kPlayerPickingFramePathPrefix, i));
        tex->setSmooth(true);
        pickingFrames.push_back(tex);
    }

    texturesLoaded = !runFrames.empty() &&
        runFrames[0]->getSize().x > 0 && runFrames[0]->getSize().y > 0 &&
        !jumpFrames.empty() &&
        jumpFrames[0]->getSize().x > 0 && jumpFrames[0]->getSize().y > 0 &&
        !pickingFrames.empty() &&
        pickingFrames[0]->getSize().x > 0 && pickingFrames[0]->getSize().y > 0 &&
        idleTexture->getSize().x > 0 && idleTexture->getSize().y > 0;

    if (texturesLoaded)
    {
        sprite.setScale(kPlayerGlobalScale, kPlayerGlobalScale);
        applyTexture(*idleTexture); // start in the idle pose
    }

    updateSpritePosition();
}

void Player::applyTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true); // true: reset texture rect to the new texture's full size
    float width = static_cast<float>(texture.getSize().x);
    float height = static_cast<float>(texture.getSize().y);
    sprite.setOrigin(width / 2.f, height * kPlayerBaselineRatio);
}

void Player::update(float deltaTime)
{
    // Falling uses extra gravity so jumps feel snappier on the way down.
    if (velocityY < 0.f)
        velocityY += kGravity * deltaTime;
    else
        velocityY += kGravity * kFallGravityMultiplier * deltaTime;

    hitbox.move(0.f, velocityY * deltaTime);

    if (hitbox.getPosition().y > kPlayerGroundY)
    {
        hitbox.setPosition(hitbox.getPosition().x, kPlayerGroundY);
        velocityY = 0.f;
        onGround = true;
    }
    else
    {
        onGround = false;
    }

    if (!autoMode && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onGround)
    {
        jump();
    }

    updateSpritePosition();
}

void Player::updateAnimation(float deltaTime, bool moving, float speedMultiplier)
{
    if (!texturesLoaded) return;

    bool airborne = !onGround;

    if (airborne)
    {
       
        wasAirborne = true;
        isLandingSettle = false; 
        picking = false; 

        float currentY = hitbox.getPosition().y;
        float heightProgress; 
        int startFrame, endFrame;

        if (velocityY < 0.f)
        {
            // Rising: 0 at the ground, 1 at the peak.
            float heightRisen = kPlayerGroundY - currentY;
            heightProgress = heightRisen / kPlayerJumpRiseHeight;
            startFrame = 0;
            endFrame = kPlayerJumpPeakFrameIndex;
        }
        else
        {
            // Falling: 0 at the peak, 1 at ground contact.
            float heightAboveGround = kPlayerGroundY - currentY;
            heightProgress = 1.f - (heightAboveGround / kPlayerJumpRiseHeight);
            startFrame = kPlayerJumpPeakFrameIndex;
            endFrame = kPlayerJumpGroundContactFrameIndex;
        }

        if (heightProgress < 0.f) heightProgress = 0.f;
        if (heightProgress > 1.f) heightProgress = 1.f;

        int frameSpan = endFrame - startFrame;
        jumpFrame = startFrame + static_cast<int>(heightProgress * static_cast<float>(frameSpan));
        if (jumpFrame > endFrame) jumpFrame = endFrame;
        if (jumpFrame < startFrame) jumpFrame = startFrame;

        applyTexture(*jumpFrames[jumpFrame]);
        return;
    }


    if (wasAirborne)
    {
        wasAirborne = false;
        isLandingSettle = true;
        landingSettleTimer = 0.f;
    }

    if (isLandingSettle)
    {
        landingSettleTimer += deltaTime;
        float t = landingSettleTimer / kPlayerJumpLandingSettleDuration;

        if (t >= 1.f)
        {
            t = 1.f;
            isLandingSettle = false; 
        }

        int startFrame = kPlayerJumpGroundContactFrameIndex;
        int endFrame = kPlayerJumpFrameCount - 1;
        int frameSpan = endFrame - startFrame;

        jumpFrame = startFrame + static_cast<int>(t * static_cast<float>(frameSpan));
        if (jumpFrame > endFrame) jumpFrame = endFrame;

        applyTexture(*jumpFrames[jumpFrame]);
        return;
    }

   
    if (picking)
    {
        if (!pickingHolding)
        {
            pickingTimer += deltaTime;
            int frame = static_cast<int>(pickingTimer / kPlayerPickingFrameTime);

            if (frame >= kPlayerPickingFrameCount)
            {
                pickingFrame = kPlayerPickingFrameCount - 1; 
                pickingHolding = true;
                pickingHoldTimer = 0.f;
            }
            else
            {
                pickingFrame = frame;
            }

            applyTexture(*pickingFrames[pickingFrame]);
            return;
        }
        else
        {
            pickingHoldTimer += deltaTime;
            applyTexture(*pickingFrames[kPlayerPickingFrameCount - 1]);

            if (pickingHoldTimer >= kPlayerPickingHoldDuration)
            {
                picking = false;
                pickingHolding = false; 
            }
            else
            {
                return;
            }
        }
    }

    if (moving)
    {
        if (!isMoving)
        {
            currentFrame = 0;
            animTimer = 0.f;
        }
        isMoving = true;

        animTimer += deltaTime;

        float effectiveFrameTime = kPlayerFrameTime / speedMultiplier;
        while (animTimer >= effectiveFrameTime)
        {
            animTimer -= effectiveFrameTime;
            currentFrame = (currentFrame + 1) % kPlayerFrameCount;
        }

        applyTexture(*runFrames[currentFrame]);
    }
    else
    {
        animTimer = 0.f;
        isMoving = false;
        applyTexture(*idleTexture);
    }
}

void Player::jump()
{
    velocityY = kJumpStrength;
}

void Player::triggerPickup()
{
    picking = true;
    pickingFrame = 0;
    pickingTimer = 0.f;
    pickingHolding = false;
    pickingHoldTimer = 0.f;
}

void Player::forceGrounded()
{
    hitbox.setPosition(hitbox.getPosition().x, kPlayerGroundY);
    velocityY = 0.f;
    onGround = true;
    updateSpritePosition();
}

void Player::reset()
{
    hitbox.setPosition(kPlayerStartX, kPlayerStartY); 
    velocityY = 0.f;
    onGround = false;

    currentFrame = 0;
    animTimer = 0.f;
    isMoving = false;

    jumpFrame = 0;
    wasAirborne = false;
    isLandingSettle = false;
    landingSettleTimer = 0.f;

    picking = false;
    pickingFrame = 0;
    pickingTimer = 0.f;
    pickingHolding = false;
    pickingHoldTimer = 0.f;

    if (texturesLoaded) applyTexture(*idleTexture);
    updateSpritePosition();
}

void Player::draw(sf::RenderWindow& window)
{
    if (texturesLoaded)
        window.draw(sprite);
    else
        window.draw(hitbox);
}

void Player::setAutoMode(bool enabled)
{
    autoMode = enabled;
}

void Player::updateAuto(float deltaTime, const std::vector<sf::RectangleShape>& rocks)
{
    update(deltaTime);

    for (const auto& rock : rocks)
    {
        float distance = rock.getPosition().x - hitbox.getPosition().x;
        if (distance > 0.f && distance < kPlayerAutoJumpDistance && onGround)
        {
            jump();
            break;
        }
    }
}

void Player::updateSpritePosition()
{
    if (!texturesLoaded) return;

    sprite.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f,
        hitbox.getPosition().y + hitbox.getSize().y + kPlayerSpriteYOffset
    );
}

sf::FloatRect Player::getGlobalBounds() const
{
    return hitbox.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return hitbox.getPosition();
}