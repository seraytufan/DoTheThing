#include "CountdownTimer.h"
#include <iomanip>
#include <sstream>

void CountdownTimer::init(sf::Font& font, unsigned int characterSize, sf::Color color, float totalSeconds)
{
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
    remainingSeconds = totalSeconds;
    refreshDisplayString();
}

void CountdownTimer::setPosition(float x, float y)
{
    text.setPosition(x, y);
}

void CountdownTimer::reset(float totalSeconds)
{
    remainingSeconds = totalSeconds;
    refreshDisplayString();
}

void CountdownTimer::update(float deltaTime, bool isRunning)
{
    if (isRunning && remainingSeconds > 0.f)
    {
        remainingSeconds -= deltaTime;
        if (remainingSeconds < 0.f)
        {
            remainingSeconds = 0.f;
        }
    }

    refreshDisplayString();
}

void CountdownTimer::refreshDisplayString()
{
    
    int totalWholeSeconds = static_cast<int>(remainingSeconds + 0.0001f);
    int minutes = totalWholeSeconds / 60;
    int seconds = totalWholeSeconds % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds;
    text.setString(oss.str());
}

void CountdownTimer::draw(sf::RenderWindow& window)
{
    window.draw(text);
}