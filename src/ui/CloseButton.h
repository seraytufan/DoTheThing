#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


class CloseButton
{
public:
    
    void init(std::shared_ptr<sf::Texture> normalTexture,
        std::shared_ptr<sf::Texture> pressedTexture,
        float targetWidth, sf::Vector2f topRightCorner, sf::Vector2f inset);

  
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

   
    void update(const sf::Vector2f& mousePos);

    void draw(sf::RenderWindow& window);

   
    bool wasClicked();

private:
    void applyTexture(const std::shared_ptr<sf::Texture>& texture, float width);
    bool contains(const sf::Vector2f& point) const;

    std::shared_ptr<sf::Texture> normalTexture;
    std::shared_ptr<sf::Texture> pressedTexture;
    sf::Sprite sprite;

    float baseWidth = 0.f;
    sf::Vector2f center;

    bool pressed = false;     
    bool clickedFlag = false; 
};
