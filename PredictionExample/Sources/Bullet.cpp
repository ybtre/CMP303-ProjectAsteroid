#include "../Headers/Bullet.h"

Bullet::Bullet() {
    name = "bullet";
}

void Bullet::Update() {
    dx = cos(angle * DEGTORAD) * 6;
    dy = sin(angle * DEGTORAD) * 6;
    // angle+=rand()%6-3;
    x += dx;
    y += dy;

    if (x > WIDTH || x < 0 || y > HEIGHT || y < 0) life = 0;
}
