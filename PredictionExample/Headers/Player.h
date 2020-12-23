#pragma once

#include <initializer_list>

#include "Macros.h"
#include "Entity.h"


class Player : public Entity {
public:
    bool thrust;

    Player();

    void Update(float deltaTime);

    void SetPlayerPos_X(float x) { dx = x;}
    void SetPlayerPos_Y(float y) { dx = y;}
    
    float GetPlayerPosition_X(){ return x; }
    float GetPlayerPosition_Y(){ return y; }
};
