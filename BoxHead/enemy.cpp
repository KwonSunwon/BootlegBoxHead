#include "enemy.h"

void Enemy::Move_left() { location.x -= speed; }
void Enemy::Move_right() { location.x += speed; }
void Enemy::Move_up() { location.y -= speed; }
void Enemy::Move_down() { location.y += speed; }

void Enemy::Set_link(Enemy* _link) 
{
	link = _link;
}
Enemy* Enemy::Get_link() {
	return link;
}

void Enemy::Set_id(int _id) { id = _id; }
int Enemy::Get_id() { return id; }

void Enemy::Init_enemy(int _id) {
	switch (_id)
	{
	case MOB1:
		Set_Speed(M1_SP);
		Set_Health(M1_HP);
		Set_Attack(M1_ATK);
		break;

	case MOB2:
		Set_Speed(M2_SP);
		Set_Health(M2_HP);
		Set_Attack(M2_ATK);
		break;

	case MOB3:
		Set_Speed(M3_SP);
		Set_Health(M3_HP);
		Set_Attack(M3_ATK);
		break;

	case MOB4:
		Set_Speed(M4_SP);
		Set_Health(M4_HP);
		Set_Attack(M4_ATK);
		break;

	case BOSS:
		Set_Speed(BOSS_SP);
		Set_Health(BOSS_HP);
		Set_Attack(BOSS_ATK);
		break;
	}
}