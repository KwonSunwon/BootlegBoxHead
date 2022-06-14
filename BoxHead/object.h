#pragma once
#include "stdafx.h"

#define OBJECT_X_SIZE 32
#define OBJECT_Y_SIZE 64
#define MAX_OBJECT_HEALTH 100

#define IDB_LEFT 202
#define IDB_RIGHT 203
#define IDB_UP 204
#define IDB_DOWN 205

class Object
{
protected:
    POINT location;
    int health;
    int attack;
    int speed;
    HBITMAP image;
    BITMAP info;

public:
    void Move_left() {}
    void Move_right() {}
    void Move_up() {}
    void Move_down() {}

    void Set_Image(HBITMAP);
    void Set_Info();
    void Set_Location(POINT);
    void Set_Health(int);
    void Set_Attack(int);
    void Set_Speed(int);

    int Get_Health();
    int Get_Attack();
    int Get_Speed();
    POINT Get_Location();
    POINT Get_Position();
    HBITMAP Get_Image();
    BITMAP Get_Info();

    RECT Get_Rect();

    void Get_Heal(int);
    void Get_Damage(int);
    void Attack(Object);
};
