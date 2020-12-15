#pragma once

#include <initializer_list>

#include "Macros.h"
#include "Entity.h"


class Player : public Entity {
public:
    bool thrust;

    Player();

    void Update(float deltaTime);

    float GetPlayerPosition_X(){ return x; }
    float GetPlayerPosition_Y(){ return y; }
};
