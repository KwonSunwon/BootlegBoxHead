#include "enemy.h"

void Enemy::Move_left() { location.x -= speed; }
void Enemy::Move_right() { location.x += speed; }
void Enemy::Move_up() { location.y -= speed; }
void Enemy::Move_down() { location.y += speed; }

void Enemy::Set_id(int _id) { id = _id; }
int Enemy::Get_id() { return id; }

void Enemy::Init_enemy(int _id)
{
    switch (_id)
    {
    case MOB1:
        Set_Speed(ENEMY_STD_SPEED);
        Set_Health(M1_HP);
        Set_Attack(M1_ATK);
        Set_id(_id);
        break;

    case MOB2:
        Set_Speed(ENEMY_STD_SPEED);
        Set_Health(M2_HP);
        Set_Attack(M2_ATK);
        Set_id(_id);
        break;

    case MOB3:
        Set_Speed(ENEMY_STD_SPEED);
        Set_Health(M3_HP);
        Set_Attack(M3_ATK);
        Set_id(_id);
        break;

    case MOB4:
        Set_Speed(ENEMY_STD_SPEED);
        Set_Health(M4_HP);
        Set_Attack(M4_ATK);
        Set_id(_id);
        break;

    case BOSS:
        Set_Speed(ENEMY_STD_SPEED);
        Set_Health(BOSS_HP);
        Set_Attack(BOSS_ATK);
        Set_id(_id);
        break;
    }
}