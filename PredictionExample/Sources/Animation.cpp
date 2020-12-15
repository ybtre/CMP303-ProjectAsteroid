#include "../Headers/Animation.h"

Animation::Animation() {
}

Animation::Animation(sf::Texture& animeTexture, int x, int y, int width, int height, int count, float Speed) {
    Frame = 0;
    speed = Speed;

    for (int i = 0; i < count; i++)
        frames.push_back(sf::IntRect(x + i * width, y, width, height));

    sprite.setTexture(animeTexture);
    sprite.setOrigin(width / 2, height / 2);
    sprite.setTextureRect(frames[0]);
}

void Animation::Update(float deltaTime) {
    Frame += speed;
    int n = frames.size(); // no idea
    if (Frame >= n) {
        Frame -= n;
    }
    if (n > 0) {
        sprite.setTextureRect(frames[static_cast<int>(Frame)]);
    }

}

bool Animation::isEnd() {
    // no clue again
    return Frame + speed >= frames.size();
}
