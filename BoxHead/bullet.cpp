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
	_target.Get_Damage(damage);
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
}

Bullet* Bullet::Get_Rlink() { return Rlink; }

Bullet* Bullet::Get_Llink() { return Llink; }