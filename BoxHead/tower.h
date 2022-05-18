#pragma once
#include "head_library.h"
#include "object.h"

class Tower : Object
{
private:
public:
	Tower(int _health, int _attack, int _speed, POINT Location) : Object(_health, _attack,_speed,Location) {}
};
