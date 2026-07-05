#pragma once

#include "../core/GameState.h"
#include "../mechanics/Grounding.h"


class GroundingState : public GameState
{
public:
   
    void init(sf::Font& fontRef);

    void onEnter() override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

    bool isFinished() const { return grounding.isFinished(); }

private:
    Grounding grounding;
};
