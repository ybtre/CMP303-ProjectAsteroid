#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Animation {
public:
    float Frame, speed;
    sf::Sprite sprite;
    std::vector<sf::IntRect> frames;

    Animation();
    
    Animation(sf::Texture& animeTexture, int x, int y, int width, int height, int count, float Speed); 

    void Update(float deltaTime);

    bool isEnd();

};
