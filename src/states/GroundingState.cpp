#include "GroundingState.h"





void GroundingState::init(sf::Font& fontRef)
{
    (void)fontRef; // Grounding is now fully image-based and no longer needs a font
    grounding.init();
}

void GroundingState::onEnter()
{
    // Always start the breathing cycle fresh, even on a repeat visit.
    grounding.reset();
}

void GroundingState::update(float deltaTime, sf::RenderWindow& window)
{
    (void)window; // Grounding only needs deltaTime -- it polls SPACE directly
    grounding.update(deltaTime);
}

void GroundingState::render(sf::RenderWindow& window)
{
    grounding.draw(window);
}