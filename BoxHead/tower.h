#pragma once
#include "stdafx.h"
#include "object.h"

#define HEAL_TOWER_ID 1050 
#define SNIPE_TOWER_ID 1051
#define BOMB_TOWER_ID 1052
#define STD_TOWER_ID 1053

#define HEAL_AMOUNT 10
#define HEAL_DISTANCE 45

class Tower : Object
{
private:
	int id;
public:
	int Get_id();
	void Operate(Object);
	void Heal_target(Player);
};
