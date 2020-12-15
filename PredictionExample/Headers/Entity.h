#pragma once
#include <string>
#include "Animation.h"

class Entity {
public:
    float x, y, dx, dy, mRadius, angle;
    bool life;
    std::string name;
    Animation anim;

    Entity();

    void Init(Animation& animation, int X, int Y, float Angle = 0, int radius = 1);

    virtual void Update();

    void Render(sf::RenderWindow& app);

    virtual ~Entity();
};
