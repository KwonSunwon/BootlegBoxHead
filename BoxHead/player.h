#pragma once
#include "head_library.h"
#include "object.h"

class Player : public Object
{
public:
	Player(int _health, int _attack, int _speed, POINT firstLocation) : Object(_health, _attack,_speed,firstLocation) {}

    virtual void Move_left() { location.x -= speed; }

    virtual void Move_right() { location.x += speed; }

    virtual void Move_up() { location.y -= speed; }

    virtual void Move_down() { location.y += speed; }
};

