#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class HeartDisplay
{
public:
    void init(int totalLives, sf::Vector2f position, float targetWidth, float spacing,
        const std::string& fullHeartPath, const std::string& lostHeartPath);

    void setLivesRemaining(int lives);

    void draw(sf::RenderWindow& window);

private:
    void applyTexture(sf::Sprite& heart, const sf::Texture& texture) const;

    std::shared_ptr<sf::Texture> fullTexture;
    std::shared_ptr<sf::Texture> lostTexture;
    std::vector<sf::Sprite> hearts;
    float targetWidth = 1.f;
    int livesRemaining = 0;
};