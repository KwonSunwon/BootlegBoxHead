#include "stdafx.h"
#include "bullet.h"
#include "enemy.h"

void Bullet::Shot_bullet(int _damage, int _way, POINT shot_pos)
{
	damage = _damage; 
	way = _way;
	location = shot_pos;
}

void Bullet::Deliver_damage(Enemy* _target)
{
	POINT e_pos = _target->Get_Location();
	
	_target->Get_Damage(damage);

	switch (way)
	{
	case IDB_UP:
		e_pos.y -= knockback;

		_target->Set_Location(e_pos);
		break;
	case IDB_DOWN:
		e_pos.y += knockback;

		_target->Set_Location(e_pos);
		break;
	case IDB_LEFT:
		e_pos.x -= knockback;

		_target->Set_Location(e_pos);
		break;
	case IDB_RIGHT:
		e_pos.x += knockback;

		_target->Set_Location(e_pos);
		break;
	}
}

void Bullet::Set_Llink(Bullet* prev)
{
	Llink = prev;
}

void Bullet::Set_Rlink(Bullet* next)
{
	Rlink = next;
}

void Bullet::Set_Type(int _type)
{
	type = _type;

	switch (_type)
	{
	case STD_BULLET:
		knockback = 5;
		break;
	case SNIPE_BULLET:
		knockback = 10;
		break;
	case SHOTGUN_BULLET:
		knockback = 25;
		break;
	case RIFLE_BULLET:
		knockback = 2;
		break;
	}
}

void Bullet::Set_Location(POINT _location)
{
	location = _location;
}

Bullet* Bullet::Get_Rlink() { return Rlink; }

Bullet* Bullet::Get_Llink() { return Llink; }

int Bullet::Get_way() { return way; }

POINT Bullet::Get_Location() { return location; }

int Bullet::Get_type() { return type; }