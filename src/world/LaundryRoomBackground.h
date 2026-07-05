#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "ParallaxLayer.h"



class LaundryRoomBackground
{
public:
    void init();

  
    void update(float deltaTime, float pixels, bool worldMoving);

    void draw(sf::RenderWindow& window);

private:
    enum class DecorationType { ShelfPair, Cabinet, Window };

    void spawnDecorationIfDue(float deltaTime, bool worldMoving);
    void recycleMachines();

    ParallaxLayer wall;
    ParallaxLayer floor;

   
    std::shared_ptr<sf::Texture> shelfTexture1;
    std::shared_ptr<sf::Texture> shelfTexture2;
    std::shared_ptr<sf::Texture> cabinetTexture;
    std::shared_ptr<sf::Texture> windowTexture;
    std::vector<sf::Sprite> decorations;
    float decorationSpawnTimer = 0.f;
    float nextDecorationSpawn = 0.f;

   
    DecorationType lastSpawnedType = DecorationType::Window;
    bool hasSpawnedBefore = false;

    std::shared_ptr<sf::Texture> machineTexture;
    std::vector<sf::Sprite> machines;
};