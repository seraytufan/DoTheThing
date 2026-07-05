#include "TextHelper.h"

namespace TextHelper
{
    void centerOrigin(sf::Text& text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }

    void centerAt(sf::Text& text, float x, float y)
    {
        centerOrigin(text);
        text.setPosition(x, y);
    }

    void fitToBox(sf::Text& text, float maxWidth, float maxHeight, unsigned int minCharacterSize)
    {
        sf::FloatRect bounds = text.getLocalBounds();

        while ((bounds.width > maxWidth || bounds.height > maxHeight) &&
            text.getCharacterSize() > minCharacterSize)
        {
            text.setCharacterSize(text.getCharacterSize() - 1);
            bounds = text.getLocalBounds();
        }
    }
}