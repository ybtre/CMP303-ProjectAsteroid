#pragma once

#include <initializer_list>

#include "Macros.h"
#include "Entity.h"
#include <iostream>


class Player : public Entity {
public:
    bool thrust = false;;

    Player();

    void Update(float deltaTime);

    void SetPlayerPos_X(float newX) { x = newX;}
    void SetPlayerPos_Y(float newY) { y = newY;}
    
    float GetPlayerPosition_X(){ return x; }
    float GetPlayerPosition_Y(){ return y; }
};
