#include "../Headers/Entity.h"

Entity::Entity() {
    life = 1;
}

void Entity::Init(Animation& animation, int X, int Y, float Angle, int radius) {
    anim = animation;
    x = X;
    y = Y;
    angle = Angle;
    mRadius = radius;
}

void Entity::Update() {
}

void Entity::Render(sf::RenderWindow& app) {
    anim.sprite.setPosition(x, y);
    anim.sprite.setRotation(angle + 90);
    app.draw(anim.sprite);

    sf::CircleShape circle(mRadius);
    circle.setFillColor(sf::Color(255, 0, 0, 170));
    circle.setPosition(x, y);
    circle.setOrigin(mRadius, mRadius);
}

Entity::~Entity() {
};
