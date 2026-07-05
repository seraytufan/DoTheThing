#include "LaundryRoomBackground.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"
#include <cstdlib>

using namespace Constants;

namespace
{
   
    constexpr float kWallSpeedRatio = 0.55f;  
    constexpr float kFloorSpeedRatio = 0.95f; 
    

    
    constexpr int kShelfPairWeight = 2;
    constexpr int kCabinetWeight = 2;  
    constexpr int kWindowWeight = 3;    
    constexpr int kDecorationTotalWeight = kShelfPairWeight + kCabinetWeight + kWindowWeight;

    constexpr float kDecorationSpawnX = 1280.f;
    constexpr float kDecorationOffscreenX = -600.f;
    constexpr float kDecorationInitialSpawnBase = 1.0f;
    constexpr int kDecorationInitialSpawnRandRange = 2;  
    constexpr float kDecorationRespawnBase = 1.8f;
    constexpr int kDecorationRespawnRandRange = 2;       

    constexpr float kShelfScale = 0.20f; 
    constexpr float kShelfTopY = 100.f;
    constexpr float kShelfBottomY = 210.f; 

  
    constexpr float kCabinetScale = 0.75f;   
    constexpr float kCabinetY = 110.f;        
    constexpr float kCabinetXOffset = 260.f; 

    
    constexpr float kWindowScale = 0.70f; 
    constexpr float kWindowY = 70.f;

   
    constexpr int kMachineCount = 5;
    constexpr float kMachineScale = 0.276f;
    constexpr float kMachineSpacing = 347.f;
    constexpr float kMachineStartX = 40.f;
    constexpr float kMachineY = 255.f;
    constexpr float kMachineRecycleThresholdX = -350.f;
}

void LaundryRoomBackground::init()
{
    wall.init(kWallTexturePath, 0.f, kFloorBoundaryY, static_cast<float>(kWindowWidth));
    floor.init(kFloorTexturePath, kFloorBoundaryY, static_cast<float>(kWindowHeight) - kFloorBoundaryY,
        static_cast<float>(kWindowWidth));

    shelfTexture1 = TextureManager::instance().acquire(kShelfTexture1Path);
    shelfTexture1->setSmooth(true);
    shelfTexture2 = TextureManager::instance().acquire(kShelfTexture2Path);
    shelfTexture2->setSmooth(true);
    cabinetTexture = TextureManager::instance().acquire(kCabinetTexturePath);
    cabinetTexture->setSmooth(true);
    windowTexture = TextureManager::instance().acquire(kWindowTexturePath);
    windowTexture->setSmooth(true);

    nextDecorationSpawn = kDecorationInitialSpawnBase +
        static_cast<float>(rand() % kDecorationInitialSpawnRandRange);

    machineTexture = TextureManager::instance().acquire(kMachineTexturePath);
    for (int i = 0; i < kMachineCount; ++i)
    {
        sf::Sprite machine;
        machine.setTexture(*machineTexture);
        machine.setScale(kMachineScale, kMachineScale);
        machine.setPosition(kMachineStartX + i * kMachineSpacing, kMachineY);
        machines.push_back(machine);
    }
}

void LaundryRoomBackground::spawnDecorationIfDue(float deltaTime, bool worldMoving)
{
    if (worldMoving)
    {
        decorationSpawnTimer += deltaTime;
    }

    if (decorationSpawnTimer >= nextDecorationSpawn)
    {
        decorationSpawnTimer = 0.f;
        nextDecorationSpawn = kDecorationRespawnBase + static_cast<float>(rand() % kDecorationRespawnRandRange);

        
        DecorationType chosen;
        do
        {
            int roll = rand() % kDecorationTotalWeight;
            if (roll < kShelfPairWeight)
            {
                chosen = DecorationType::ShelfPair;
            }
            else if (roll < kShelfPairWeight + kCabinetWeight)
            {
                chosen = DecorationType::Cabinet;
            }
            else
            {
                chosen = DecorationType::Window;
            }
        } while (hasSpawnedBefore && chosen == lastSpawnedType);

        lastSpawnedType = chosen;
        hasSpawnedBefore = true;

        if (chosen == DecorationType::Window)
        {
            
            sf::Sprite windowSprite;
            windowSprite.setTexture(*windowTexture);
            windowSprite.setScale(kWindowScale, kWindowScale);
            windowSprite.setPosition(kDecorationSpawnX, kWindowY);
            decorations.push_back(windowSprite);
        }
        else if (chosen == DecorationType::Cabinet)
        {
           
            sf::Sprite cabinet;
            cabinet.setTexture(*cabinetTexture);
            cabinet.setScale(kCabinetScale, kCabinetScale);
            cabinet.setPosition(kDecorationSpawnX + kCabinetXOffset, kCabinetY);
            decorations.push_back(cabinet);
        }
        else
        {
          
            bool firstOnTop = (rand() % 2) == 0;

            sf::Sprite topShelf;
            topShelf.setTexture(firstOnTop ? *shelfTexture1 : *shelfTexture2);
            topShelf.setScale(kShelfScale, kShelfScale);
            topShelf.setPosition(kDecorationSpawnX, kShelfTopY);
            decorations.push_back(topShelf);

            sf::Sprite bottomShelf;
            bottomShelf.setTexture(firstOnTop ? *shelfTexture2 : *shelfTexture1);
            bottomShelf.setScale(kShelfScale, kShelfScale);
            bottomShelf.setPosition(kDecorationSpawnX, kShelfBottomY);
            decorations.push_back(bottomShelf);
        }
    }

    for (int i = static_cast<int>(decorations.size()) - 1; i >= 0; --i)
    {
        if (decorations[i].getPosition().x < kDecorationOffscreenX)
        {
            decorations.erase(decorations.begin() + i);
        }
    }
}

void LaundryRoomBackground::recycleMachines()
{
   
    for (auto& machine : machines)
    {
        if (machine.getPosition().x < kMachineRecycleThresholdX)
        {
            float farthestX = machines[0].getPosition().x;
            for (auto& other : machines)
            {
                if (other.getPosition().x > farthestX)
                {
                    farthestX = other.getPosition().x;
                }
            }
            machine.setPosition(farthestX + kMachineSpacing, machine.getPosition().y);
        }
    }
}

void LaundryRoomBackground::update(float deltaTime, float pixels, bool worldMoving)
{
    if (worldMoving)
    {
        wall.scroll(pixels * kWallSpeedRatio);
        floor.scroll(pixels * kFloorSpeedRatio);

        for (auto& decoration : decorations)
        {
            decoration.move(-pixels * kWallSpeedRatio, 0.f);
        }
        for (auto& machine : machines)
        {
            machine.move(-pixels, 0.f); 
        }
    }

    
    spawnDecorationIfDue(deltaTime, worldMoving);
    recycleMachines();
}

void LaundryRoomBackground::draw(sf::RenderWindow& window)
{
    wall.draw(window);

    for (auto& decoration : decorations)
    {
        window.draw(decoration);
    }

    floor.draw(window);

    for (auto& machine : machines)
    {
        window.draw(machine);
    }
}