#include "../Headers/Player.h"

Player::Player() {
    name = "player";
}

void Player::Update(float deltaTime) {
    if (thrust) {
        dx += cos(angle * DEGTORAD) * 6;
        dy += sin(angle * DEGTORAD) * 6;
    }
    else { // natural slow down of speed over time if player is not accelarating 
        dx *= 0.99;
        dy *= 0.99;
    }

    int maxSpeed = 300;
    float speed = sqrt(dx * dx + dy * dy);
    if (speed > maxSpeed) {
        dx *= maxSpeed / speed;
        dy *= maxSpeed / speed;
    }

    x += dx * deltaTime;
    y += dy * deltaTime;

    if (x > WIDTH) x = 0;
    if (x < 0) x = WIDTH;
    if (y > HEIGHT) y = 0;
    if (y < 0) y = HEIGHT;
}
