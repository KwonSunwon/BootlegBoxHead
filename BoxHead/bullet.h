#pragma once
#include "stdafx.h"
#include "enemy.h"

#define STD_BULLET 500
#define SHOTGUN_BULLET 502
#define RIFLE_BULLET 503
#define SNIPE_BULLET 501

class Bullet
{
private:
	int type;
	int damage;
	int way;
	int knockback;
	POINT location;
	Bullet* Llink;
	Bullet* Rlink;
public:
	void Shot_bullet(int, int, POINT);
	void Deliver_damage(Enemy*);

	void Set_Llink(Bullet*);
	void Set_Rlink(Bullet*);
	void Set_Type(int);
	void Set_Location(POINT);

	Bullet* Get_Rlink();
	Bullet* Get_Llink();
	int Get_way();
	POINT Get_Location();
	int Get_type();
};