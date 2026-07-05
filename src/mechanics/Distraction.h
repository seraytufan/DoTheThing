#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../ui/Button.h"

struct PopupBox
{
    sf::Sprite box;
    sf::Text text;
};


class Distraction
{
public:
    Distraction();

    void init(sf::Font& fontRef);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f mousePos);

    bool isChaosMode() const { return chaosMode; }
    bool isGroundingButtonVisible() const { return groundingButtonVisible; }

    // Returns true (and clears the flag) if the grounding button was clicked
    // since the last time this was checked.
    bool wasGroundingRequested();

    void reset();

private:
    void spawnPopup();

    std::vector<PopupBox> popups;
    std::shared_ptr<sf::Texture> popupTexture;

    float spawnTimer = 0.f;
    float nextSpawn = 0.f;

    int popupCount = 0;
    bool chaosMode = false;

    bool groundingRequested = false;

    sf::Font* font = nullptr;

    std::shared_ptr<sf::Texture> groundingNormalTexture;
    std::shared_ptr<sf::Texture> groundingPressedTexture;
    Button groundingButton;
    bool groundingButtonVisible = false;

    std::shared_ptr<sf::SoundBuffer> groundingClickBuffer;
    sf::Sound groundingClickSound;

    std::vector<std::string> messages;
};