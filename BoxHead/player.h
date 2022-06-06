#pragma once
#include "stdafx.h"
#include "object.h"

#define PLAYER_STDHEALTH 100
#define PLAYER_STDSPEED 10
#define PLAYER_STDATTACK 5

class Player : public Object
{
public:

    void Move_left(); 
    void Move_right();
    void Move_up(); 
    void Move_down();
};
