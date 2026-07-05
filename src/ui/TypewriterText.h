#pragma once

#include <SFML/Graphics.hpp>
#include <string>



class TypewriterText
{
public:
    void init(sf::Font& font, unsigned int characterSize, sf::Color color, float charsPerSecond);

    
    void setFullText(const std::string& text);

    
    void update(float deltaTime);

    
    void skipToEnd();

    bool isComplete() const;

    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);

private:
    sf::Text text;
    std::string fullText;
    float charsPerSecond = 30.f;
    float revealTimer = 0.f;
    std::size_t revealedCount = 0;
};
