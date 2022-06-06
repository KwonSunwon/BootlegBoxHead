#pragma once
#include "stdafx.h"
#include "object.h"

#define ENEMY_TIMER 404
#define ENEMY_TIMELAB 1000
#define ENEMY_SPAWN 300

class Enemy : public Object
{
private:
public:

    void Move_left();
    void Move_right();
    void Move_up();
    void Move_down();
};