#pragma once
#include "stdafx.h"
#include "enemy.h"

#define MAX_PISTOL 8
#define MAX_SHOTGUN 5
#define MAX_SNIPER 1
#define MAX_RIFLE 16

#define MAX_TOWERRIFLE 10

#define BULLET_SPEED 10

class Bullet
{
private:
	int damage;
	int way;
	int knockback;
	POINT location;
public:
	void Shot_bullet(int, int, POINT);
	void Deliver_damage(Enemy* );

	void Set_Location(POINT);

	int Get_way();
	POINT Get_Location();

	void Move();

	void Set_knockback(int);
};