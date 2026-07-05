#include "ProgressBar.h"
#include <algorithm>

namespace
{
    constexpr unsigned int kCapPointCount = 40; // matches the original circle smoothness
}

void ProgressBar::init(float x, float y, float width, float height, float cornerRadius,
    sf::Color trackColor, sf::Color fillColor)
{
    posX = x;
    posY = y;
    barWidth = width;
    barHeight = height;
    radius = cornerRadius;
    rounded = cornerRadius > 0.f;

    if (!rounded)
    {
        
        simpleTrack.setSize(sf::Vector2f(barWidth, barHeight));
        simpleTrack.setFillColor(trackColor);
        simpleTrack.setPosition(posX, posY);

        simpleFill.setSize(sf::Vector2f(0.f, barHeight));
        simpleFill.setFillColor(fillColor);
        simpleFill.setPosition(posX, posY);
        return;
    }

    
    const float r = radius;

    trackMid.setSize(sf::Vector2f(barWidth - barHeight, barHeight));
    trackMid.setFillColor(trackColor);
    trackMid.setPosition(posX + r, posY);

    trackLeftCap.setRadius(r);
    trackLeftCap.setPointCount(kCapPointCount);
    trackLeftCap.setFillColor(trackColor);
    trackLeftCap.setPosition(posX, posY);

    trackRightCap.setRadius(r);
    trackRightCap.setPointCount(kCapPointCount);
    trackRightCap.setFillColor(trackColor);
    trackRightCap.setPosition(posX + barWidth - barHeight, posY);

    fillMid.setFillColor(fillColor);
    fillMid.setPosition(posX + r, posY);
    fillMid.setSize(sf::Vector2f(0.f, barHeight));

    fillLeftCap.setRadius(r);
    fillLeftCap.setPointCount(kCapPointCount);
    fillLeftCap.setFillColor(fillColor);
    fillLeftCap.setPosition(posX, posY);

    fillRightCap.setRadius(r);
    fillRightCap.setPointCount(kCapPointCount);
    fillRightCap.setFillColor(fillColor);
    fillRightCap.setPosition(posX, posY);
}

void ProgressBar::setTrackOutline(sf::Color outlineColor, float outlineThickness)
{
    
    if (!rounded)
    {
        simpleTrack.setOutlineColor(outlineColor);
        simpleTrack.setOutlineThickness(outlineThickness);
    }
}

void ProgressBar::setProgress(float progress01)
{
    progress01 = std::clamp(progress01, 0.f, 1.f);
    lastProgress = progress01;

    if (!rounded)
    {
        simpleFill.setSize(sf::Vector2f(barWidth * progress01, barHeight));
        return;
    }

    float fillWidth = barWidth * progress01;

   
    if (fillWidth > 0.f && fillWidth < barHeight)
    {
        fillWidth = barHeight;
    }

    fillMid.setSize(sf::Vector2f(fillWidth - barHeight, barHeight));
    fillRightCap.setPosition(posX + fillWidth - barHeight, posY);
    
}

void ProgressBar::draw(sf::RenderWindow& window)
{
    if (!rounded)
    {
        window.draw(simpleTrack);
        window.draw(simpleFill);
        return;
    }

    window.draw(trackMid);
    window.draw(trackLeftCap);
    window.draw(trackRightCap);

   
    if (lastProgress > 0.f)
    {
        window.draw(fillMid);
        window.draw(fillLeftCap);
        window.draw(fillRightCap);
    }
}