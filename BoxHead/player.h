#pragma once
#include "head_library.h"
#include "object.h"

class Player : public Object
{
public:
	Player(int _health, int _attack, int _speed, POINT firstLocation) : Object(_health, _attack,_speed,firstLocation) {}

    void Move_left(); 
    void Move_right();
    void Move_up(); 
    void Move_down(); 
};
