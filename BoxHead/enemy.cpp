#include "enemy.h"

void Enemy::Move_left() { location.x -= speed; }
void Enemy::Move_right() { location.x += speed; }
void Enemy::Move_up() { location.y -= speed; }
void Enemy::Move_down() { location.y += speed; }