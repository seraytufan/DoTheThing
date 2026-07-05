#pragma once

#include <SFML/Graphics.hpp>



namespace TextHelper
{
    
    void centerOrigin(sf::Text& text);

    
    void centerAt(sf::Text& text, float x, float y);


    void fitToBox(sf::Text& text, float maxWidth, float maxHeight, unsigned int minCharacterSize = 8);
}