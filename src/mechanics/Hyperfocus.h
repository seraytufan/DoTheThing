#pragma once

#include <SFML/Graphics.hpp>
#include "../ui/ProgressBar.h"


class Hyperfocus
{
public:
    Hyperfocus() = default;

    void init(sf::Font& font);

    
    void update(float deltaTime, int& score);

    
    void tryActivate(int& score, int gain);

    void draw(sf::RenderWindow& window);

    
    void reset();

    bool isActive() const { return active; }

private:
    bool active = false;
    float timer = 0.f;
    int progressScore = 0;

    sf::Text label;
    ProgressBar bar;
};