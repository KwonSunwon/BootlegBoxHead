#include "player.h"

void Player::Move_left() { location.x -= speed; }

void Player::Move_right() { location.x += speed; }

void Player::Move_up() { location.y -= speed; }

void Player::Move_down() { location.y += speed; }

void Player::Set_Weapon(int _weapon)
{
	weapon_id = _weapon;
}

int Player::Get_Weapon_id() { return weapon_id; }

void Shot_Pistol(int way)
{
	switch (way)
	{
	case IDB_LEFT:
		break;
	case IDB_RIGHT:
		break;
	case IDB_UP:
		break;
	case IDB_DOWN:
		break;
	}
}

void Shot_Rifle(int way)
{
	switch (way)
	{
	case IDB_LEFT:
		break;
	case IDB_RIGHT:
		break;
	case IDB_UP:
		break;
	case IDB_DOWN:
		break;
	}
}

void Shot_Shotgun(int way)
{
	switch (way)
	{
	case IDB_LEFT:
		break;
	case IDB_RIGHT:
		break;
	case IDB_UP:
		break;
	case IDB_DOWN:
		break;
	}
}

void Shot_Sniper(int way)
{
	switch (way)
	{
	case IDB_LEFT:
		break;
	case IDB_RIGHT:
		break;
	case IDB_UP:
		break;
	case IDB_DOWN:
		break;
	}
}