#include "../Headers/Asteroid.h"

Asteroid::Asteroid() {
    dx = rand() % 8 - 4;
    dy = rand() % 8 - 4;
    name = "asteroid";
}

void Asteroid::Update() {
    x += dx;
    y += dy;

    if (x > WIDTH) x = 0;
    if (x < 0) x = WIDTH;
    if (y > HEIGHT) y = 0;
    if (y < 0) y = HEIGHT;
}
