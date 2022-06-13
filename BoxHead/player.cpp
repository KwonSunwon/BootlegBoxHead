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

void Player::Shot_Pistol(Bullet* b, int _way)
{
	Bullet* newnode;
	Bullet* tmp = b;
	newnode = (Bullet*)malloc(sizeof(Bullet));

	newnode->Shot_bullet(PISTOL_DAMAGE, _way, location);
	newnode->Set_Rlink(NULL);
	newnode->Set_Type(STD_BULLET);

	while (tmp != NULL)
	{
		tmp = tmp->Get_Rlink();
	}

	tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
}

void Player::Shot_Rifle(Bullet* b, int _way)
{
	Bullet* newnode;
	Bullet* tmp = b;
	newnode = (Bullet*)malloc(sizeof(Bullet));

	newnode->Shot_bullet(RIFLE_DAMAGE, _way, location);
	newnode->Set_Rlink(NULL);
	newnode->Set_Type(RIFLE_BULLET);

	while (tmp != NULL)
	{
		tmp = tmp->Get_Rlink();
	}

	tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
}

void Player::Shot_Shotgun(Bullet* b, int _way)
{
	Bullet* newnode;
	Bullet* tmp = b;
	newnode = (Bullet*)malloc(sizeof(Bullet));

	newnode->Shot_bullet(SHOTGUN_DAMAGE, _way, location);
	newnode->Set_Rlink(NULL);
	newnode->Set_Type(SHOTGUN_BULLET);

	while (tmp != NULL)
	{
		tmp = tmp->Get_Rlink();
	}

	tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
}

void Player::Shot_Sniper(Bullet* b, int _way)
{
	Bullet* newnode;
	Bullet* tmp = b;
	newnode = (Bullet*)malloc(sizeof(Bullet));

	newnode->Shot_bullet(SNIPER_DAMAGE, _way, location);
	newnode->Set_Rlink(NULL);
	newnode->Set_Type(SNIPE_BULLET);

	while (tmp != NULL)
	{
		tmp = tmp->Get_Rlink();
	}

	tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
}