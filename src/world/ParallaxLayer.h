#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class ParallaxLayer
{
public:
   
    void init(const std::string& texturePath, float yPosition, float targetHeight, float windowWidth);

   
    void scroll(float pixels);

    void draw(sf::RenderWindow& window);

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    float scrollX = 0.f;
    int rectWidth = 0;
};
