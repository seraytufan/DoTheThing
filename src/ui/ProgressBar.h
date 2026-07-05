#pragma once

#include <SFML/Graphics.hpp>


class ProgressBar
{
public:
  
    void init(float x, float y, float width, float height, float cornerRadius,
        sf::Color trackColor, sf::Color fillColor);

    
    void setTrackOutline(sf::Color outlineColor, float outlineThickness);

    
    void setProgress(float progress01);

    void draw(sf::RenderWindow& window);

private:
    float posX = 0.f, posY = 0.f, barWidth = 0.f, barHeight = 0.f, radius = 0.f;
    bool rounded = false;
    float lastProgress = 0.f; 

   
    sf::RectangleShape simpleTrack;
    sf::RectangleShape simpleFill;

  
    sf::RectangleShape trackMid;
    sf::CircleShape trackLeftCap;
    sf::CircleShape trackRightCap;

    sf::RectangleShape fillMid;
    sf::CircleShape fillLeftCap;
    sf::CircleShape fillRightCap;
};
