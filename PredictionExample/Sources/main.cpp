#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <SFML/Audio.hpp>
#include <iostream>

#include "../Headers/Macros.h"
#include "../Headers/Animation.h"
#include "../Headers/Entity.h"
#include "../Headers/Player.h"
#include "../Headers/Asteroid.h"
#include "../Headers/Bullet.h"
#include <vector>


// Collision detection //
bool hasCollided(Entity* a, Entity* b) {
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->mRadius + b->mRadius) * (a->mRadius + b->mRadius);
}


int main() {
    srand(time(nullptr));

    // Lifes and Score //
    int lives = 30;
    int score = 0;

    sf::Font hudFont;
    if (!hudFont.loadFromFile("images/Fonts/Magettas Regular DEMO.otf"))
        return 0;

    // setting the text HUD //
    sf::Text lifeLabel, scoreLabel, livesLeft, totalScore, playerText;
    lifeLabel.setFont(hudFont);
    lifeLabel.setString("Lives Remaining: ");
    lifeLabel.setCharacterSize(16);
    lifeLabel.setPosition(10, 10);
    lifeLabel.setFillColor(sf::Color::White);

    scoreLabel.setFont(hudFont);
    scoreLabel.setString("Score: ");
    scoreLabel.setCharacterSize(16);
    scoreLabel.setPosition(10, 40);
    scoreLabel.setFillColor(sf::Color::White);

    livesLeft.setFont(hudFont);
    livesLeft.setString(std::to_string(lives));
    livesLeft.setCharacterSize(16);
    livesLeft.setPosition(145, 10);
    livesLeft.setFillColor(sf::Color::White);

    totalScore.setFont(hudFont);
    totalScore.setString(std::to_string(score));
    totalScore.setCharacterSize(16);
    totalScore.setPosition(65, 40);
    totalScore.setFillColor(sf::Color::White);

sf:sf::String playerInput;
    playerText.setFont(hudFont);
    playerText.setString("");
    playerText.setCharacterSize(16);
    playerText.setPosition(10, 60);
    playerText.setFillColor(sf::Color::Red);

    // Window rendering and naming //
    sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT), "Asteroids!");
    app.setFramerateLimit(60);


    // Textures
    sf::Texture PlayerTexture, Player2Texture, BackgroundTexture, Explosion_A_Texture, AsteroidTexture, BulletTexture,
                AsteroidSmallTexture, Explosion_B_Texture, HmTexture;

    PlayerTexture.loadFromFile("images/spaceship.jpeg");
    Player2Texture.loadFromFile("images/spaceship2.jpeg");
    BackgroundTexture.loadFromFile("images/background.jpeg");
    Explosion_A_Texture.loadFromFile("images/explosions/type_C.png");
    AsteroidTexture.loadFromFile("images/rock.png");
    BulletTexture.loadFromFile("images/fire_blue.png");
    AsteroidSmallTexture.loadFromFile("images/rock_small.png");
    Explosion_B_Texture.loadFromFile("images/explosions/type_B.png");
    HmTexture.loadFromFile("images/maxresdefault.jpg");

    // makes textures less pixelated //
    PlayerTexture.setSmooth(true);
    Player2Texture.setSmooth(true);
    BackgroundTexture.setSmooth(true);
    Explosion_A_Texture.setSmooth(true);
    Explosion_B_Texture.setSmooth(true);
    AsteroidTexture.setSmooth(true);
    BulletTexture.setSmooth(true);

    sf::Sprite background(BackgroundTexture);

    // ANIMATIONS //
    Animation Explosion_A_Anim(Explosion_A_Texture, 0, 0, 256, 256, 48, 0.5);
    Animation AsteroidAnim(AsteroidTexture, 0, 0, 64, 64, 16, 0.2);
    Animation AsteroidSmallAnim(AsteroidSmallTexture, 0, 0, 64, 64, 16, 0.2);
    Animation BulletAnim(BulletTexture, 0, 0, 32, 64, 16, 0.8);
    Animation Player_ShipExplosion_Anim(Explosion_B_Texture, 0, 0, 192, 192, 64, 0.5);

    // not optimal to use a vector of objects instead of pointers to objects
    std::vector<Animation> playerAnimations;
    playerAnimations.reserve(4);

    Animation PlayerAnim(PlayerTexture, 40, 0, 40, 40, 1, 0);
    Animation Player2Anim(Player2Texture, 40, 0, 40, 40, 1, 0);
    playerAnimations.push_back(PlayerAnim);
    playerAnimations.push_back(Player2Anim);

    std::vector<Animation> playerThrustAnims;
    playerThrustAnims.reserve(4);

    Animation Player_Thrust_Anim(PlayerTexture, 40, 40, 40, 40, 1, 0);
    Animation Player2_Thrust_Anim(Player2Texture, 40, 40, 40, 40, 1, 0);
    playerThrustAnims.push_back(Player_Thrust_Anim);
    playerThrustAnims.push_back(Player2_Thrust_Anim);


    std::list<Entity*> entities;
    std::vector<Player*> playersArr;
    playersArr.reserve(3);

    for (int i = 0; i < 15; i++) {
        Asteroid* asteroid = new Asteroid();
        asteroid->Init(AsteroidAnim, rand() % WIDTH, rand() % HEIGHT, rand() % 360, 25);
        entities.push_back(asteroid);
    }

    Player* player = new Player();
    player->Init(playerAnimations[0], WIDTH / 2, HEIGHT / 2, 10, 20);
    playersArr.emplace_back(player);

    Player* player2 = new Player();
    player2->Init(playerAnimations[1], WIDTH / 2, HEIGHT / 2, 10, 20);
    playersArr.emplace_back(player2);

    // Clock for timing the deltaTime value
    sf::Clock clock;
    float gameSpeed = 1.0f;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////// MAIN LOOP //////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (app.isOpen()) {
        //Get the time since the last frame in milliseconds
        float dt = clock.restart().asSeconds() * gameSpeed;
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }

            // Fire control and logic //
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::RControl) {
                    Bullet* bullet = new Bullet();
                    bullet->Init(BulletAnim, playersArr[0]->x, playersArr[0]->y, playersArr[0]->angle, 10);
                    entities.push_back(bullet);
                }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    playerInput += event.text.unicode;
                    playerText.setString(playerInput);
                }
            }

        }

        // Movement controls //
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) playersArr[0]->angle += 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) playersArr[0]->angle -= 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) playersArr[0]->thrust = true;
        else playersArr[0]->thrust = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playersArr[1]->angle += 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playersArr[1]->angle -= 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playersArr[1]->thrust = true;
        else playersArr[1]->thrust = false;

        // Quit Game //

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            app.close();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////// Collision Detection ///////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (auto entityA : entities)
            for (auto entityB : entities) {
                if (entityA->name == "asteroid" && entityB->name == "bullet")
                    if (hasCollided(entityA, entityB)) {
                        entityA->life = false;
                        entityB->life = false;

                        score++;
                        totalScore.setString(std::to_string(score));

                        // Adding the explosion animation //
                        Entity* explosion = new Entity();
                        explosion->Init(Explosion_A_Anim, entityA->x, entityA->y);
                        explosion->name = "explosion";
                        entities.push_back(explosion);


                        // Creating smaller rocks from destroyed rocks //
                        for (int i = 0; i < 2; i++) {
                            if (entityA->mRadius == 15) continue;
                            Entity* smallAsteroid = new Asteroid();
                            smallAsteroid->Init(AsteroidSmallAnim, entityA->x, entityA->y, rand() % 360, 15);
                            entities.push_back(smallAsteroid);
                        }

                    }

                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////// Death when colliding with an Asteroid ////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                for (int i = 0; i < playersArr.size(); i++) {
                    if (playersArr[i]->name == "player" && entityB->name == "asteroid") {
                        if (hasCollided(playersArr[i], entityB)) {
                            entityB->life = false;

                            Entity* explosion = new Entity();
                            explosion->Init(Player_ShipExplosion_Anim, playersArr[i]->x, playersArr[i]->y);
                            explosion->name = "explosion";
                            entities.push_back(explosion);

                            // Player respawn point 
                            playersArr[i]->Init(playerAnimations[i], WIDTH / 2, HEIGHT / 2, 0, 20);
                            playersArr[i]->dx = 0;
                            playersArr[i]->dy = 0;

                            // Lives are reduced on death, if lives reach 0, pause screen //
                            lives--;
                            livesLeft.setString(std::to_string(lives));
                            // if (lives < 0) {
                            //     std::cout << "      GAME OVER" << std::endl << std::endl;
                            //     system("pause");
                            //     app.close();
                            // }
                        }
                    }
                }
            }


        // Player //
        for (int i = 0; i < playersArr.size(); i++) {
            if (playersArr[i]->thrust) {
                playersArr[i]->anim = playerThrustAnims[i];
            }
            else {
                playersArr[i]->anim = playerAnimations[i];
            }
        }


        // Animation for the asteroid explosion //
        for (auto entity : entities) {
            if (entity->name == "explosion") {
                if (entity->anim.isEnd()) {
                    entity->life = false;
                }
            }
        }


        // Creating smaller asteroids from destroyed ones //
        if (rand() % 150 == 0) {
            Asteroid* asteroid = new Asteroid();
            asteroid->Init(AsteroidAnim, 0, rand() % HEIGHT, rand() % 360, 25);
            entities.push_back(asteroid);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////// Update All Entities ////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (auto i = playersArr.begin(); i != playersArr.end();) {
            Player* player = *i;

            player->Update(dt);
            player->anim.Update(dt);

            if (player->life == false) {
                playersArr.erase(i);
                delete player;
            }
            else {
                ++i;
            }
        }
        for (auto i = entities.begin(); i != entities.end();) {
            Entity* entity = *i;

            entity->Update();
            entity->anim.Update(dt);

            if (entity->life == false) {
                i = entities.erase(i);
                delete entity;
            }
            else {
                ++i;
            }
        }


        // Draw //
        //app.draw(endScreen);
        app.draw(background);
        app.draw(lifeLabel);
        app.draw(livesLeft);
        app.draw(scoreLabel);
        app.draw(totalScore);
        app.draw(playerText);


        for (auto player : playersArr) {
            player->Render(app);
        }
        for (auto entity : entities) {
            entity->Render(app);
        }

        app.display();
    } // end while loop

    return 0;
}