#pragma once
#include "stdafx.h"
#include "object.h"
#include "bullet.h"

#define PLAYER_STDHEALTH 100
#define PLAYER_STDSPEED 2
#define PLAYER_STDATTACK 5

#define PISTOL 1070
#define RIFLE 1071
#define SHOTGUN 1072
#define SNIPER 1073

#define PISTOL_DAMAGE 15
#define RIFLE_DAMAGE 25
#define SHOTGUN_DAMAGE 40
#define SNIPER_DAMAGE 90

class Player : public Object
{
private:
    int weapon_id;

public:
    void Move_left();
    void Move_right();
    void Move_up();
    void Move_down();

    void Set_Weapon(int);
    int Get_Weapon_id();
};
