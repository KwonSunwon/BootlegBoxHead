#pragma once
#include "stdafx.h"
#include "object.h"

//tower id
#define HEAL_TOWER_ID 1050 
#define SNIPE_TOWER_ID 1051
#define BOMB_TOWER_ID 1052
#define STD_TOWER_ID 1053

//heal tower
#define HEAL_AMOUNT 10
#define HEAL_DISTANCE 45

//bomb tower
#define BOMB_DAMAGE 100
#define BOMB_SPLASH 30
#define BOMB_RANGE 40

class Tower : public Object
{
private:
	int id;
public:
	int Get_id();
	void Heal_target(Player);
};
