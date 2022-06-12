#pragma once
#include "stdafx.h"
#include "object.h"

#define PLAYER_STDHEALTH 100
#define PLAYER_STDSPEED 10
#define PLAYER_STDATTACK 5

#define PISTOL 1070
#define RIFLE 1071
#define SHOTGUN 1072
#define SNIPER 1073

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

    void Shot_Pistol(int);
    void Shot_Rifle(int);
    void Shot_Shotgun(int);
    void Shot_Sniper(int);
};
