#pragma once
#include "head_library.h"
#include "object.h"

class Player : public Object
{
public:
	Player(int hp, int atk, int sp) : Object(hp, atk, sp) {}
};

