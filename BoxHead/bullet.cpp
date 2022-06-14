#include "stdafx.h"
#include "bullet.h"
#include "enemy.h"

void Bullet::Shot_bullet(int _damage, int _way, POINT shot_pos)
{
	damage = _damage; 
	way = _way;
	location = shot_pos;
}

void Bullet::Deliver_damage(Enemy _target)
{
	POINT e_pos = _target.Get_Location();
	
	_target.Get_Damage(damage);

	switch (way)
	{
	case IDB_UP:
		e_pos.y -= knockback;

		_target.Set_Location(e_pos);
		break;
	case IDB_DOWN:
		e_pos.y += knockback;

		_target.Set_Location(e_pos);
		break;
	case IDB_LEFT:
		e_pos.x -= knockback;

		_target.Set_Location(e_pos);
		break;
	case IDB_RIGHT:
		e_pos.x += knockback;

		_target.Set_Location(e_pos);
		break;
	}
}

void Bullet::Set_Location(POINT _location)
{
	location = _location;
}

int Bullet::Get_way() { return way; }

POINT Bullet::Get_Location() { return location; }

void Bullet::Move()
{
	switch (way)
	{
	case IDB_UP:
		location.y -= BULLET_SPEED;
		break;
	case IDB_DOWN:
		location.y += BULLET_SPEED;
		break;
	case IDB_LEFT:
		location.x -= BULLET_SPEED;
		break;
	case IDB_RIGHT:
		location.x += BULLET_SPEED;
		break;
	}
}