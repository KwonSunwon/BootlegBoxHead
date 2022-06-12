#pragma once
#include "stdafx.h"
#include "object.h"

#define ENEMY_STD_SPEED 3

#define ENEMY_TIMER 404
#define ENEMY_TIMELAB 100
#define ENEMY_SPAWN 300
#define ENEMY_MAXCOUNT 100

//id 값
#define MOB1 1001
#define MOB2 1002
#define MOB3 1003
#define MOB4 1004
#define BOSS 1005 

//몹 별 체력
#define M1_HP 100
#define M2_HP 80
#define M3_HP 150
#define M4_HP 120
#define BOSS_HP 800

//몹 별 속도
#define M1_SP 200
#define M2_SP 180
#define M3_SP 200
#define M4_SP 170
#define BOSS_SP 500

//몹 별 공격력
#define M1_ATK 5
#define M2_ATK 8
#define M3_ATK 10
#define M4_ATK 13
#define BOSS_ATK 30

class Enemy : public Object
{
private:
    Enemy* link;
    int id; //몬스터 종류 구별을 위한 아이디
public:

    void Move_left();
    void Move_right();
    void Move_up();
    void Move_down();

    void Set_link(Enemy*);
    Enemy* Get_link();

    void Set_id(int);
    int Get_id();

    void Init_enemy(int);
};

typedef struct _EnemyType
{
    Enemy* link;
    int enemy_count;
} EnemyType;