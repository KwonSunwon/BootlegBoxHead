#pragma once
#include "stdafx.h"
#include "object.h"
#include "bullet.h"

#define MAX_TOWER_COUNT 5

//tower id
#define ID_HEAL_TOWER 1050 
#define ID_SNIPE_TOWER 1051
#define ID_BOMB_TOWER 1052
#define ID_STD_TOWER 1053

//heal tower
#define HEAL_AMOUNT 10
#define HEAL_DISTANCE 45

//bomb tower
#define BOMB_DAMAGE 100
#define BOMB_SPLASH 30
#define BOMB_RANGE 40

//std tower
#define STD_DAMAGE 30

class Tower : public Object
{
private:
	int id;
	int way;
public:
	int Get_id();
	int Get_way();
	void Heal_target(Player);
	void Tower_Oparate(Bullet*);

	void Set_way(int);
	void Set_id(int);
};
