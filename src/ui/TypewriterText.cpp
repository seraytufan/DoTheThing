#include "TypewriterText.h"

void TypewriterText::init(sf::Font& font, unsigned int characterSize, sf::Color color, float speed)
{
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
    charsPerSecond = speed;
}

void TypewriterText::setFullText(const std::string& newText)
{
    fullText = newText;
    revealedCount = 0;
    revealTimer = 0.f;
    text.setString("");
}

void TypewriterText::update(float deltaTime)
{
    if (isComplete()) return;

    revealTimer += deltaTime;

    float secondsPerChar = 1.f / charsPerSecond;
    while (revealTimer >= secondsPerChar && !isComplete())
    {
        revealTimer -= secondsPerChar;
        revealedCount++;
    }

    text.setString(fullText.substr(0, revealedCount));
}

void TypewriterText::skipToEnd()
{
    revealedCount = fullText.size();
    text.setString(fullText);
}

bool TypewriterText::isComplete() const
{
    return revealedCount >= fullText.size();
}

void TypewriterText::setPosition(float x, float y)
{
    text.setPosition(x, y);
}

void TypewriterText::draw(sf::RenderWindow& window)
{
    window.draw(text);
}