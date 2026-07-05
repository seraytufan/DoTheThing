#pragma once

#include <SFML/Graphics.hpp>

class BoxTraceProgress
{
public:
    void init(float x, float y, float width, float height, float inset, float thickness, float cornerRadius, sf::Color color);

 
    void setProgress(int edgeIndex, float edgeProgress01);

    void draw(sf::RenderWindow& window);

private:
    struct Segment
    {
        sf::Vector2f start, end; 
        bool horizontal = true;
        sf::RectangleShape bar;
        sf::CircleShape startCap, endCap;
    };

    void layoutSegment(int index, float p);

    Segment segments[4];
    bool edgeVisible[4] = { false, false, false, false };
    float thickness = 0.f;
    float capRadius = 0.f;
};