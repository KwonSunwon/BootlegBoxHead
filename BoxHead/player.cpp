#include "player.h"
#include "bullet.h"

void Player::Move_left() { location.x -= speed; }

void Player::Move_right() { location.x += speed; }

void Player::Move_up() { location.y -= speed; }

void Player::Move_down() { location.y += speed; }

void Player::Set_Weapon(int _weapon)
{
	weapon_id = _weapon;
}

int Player::Get_Weapon_id() { return weapon_id; }