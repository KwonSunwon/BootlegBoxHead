#pragma once
#include "stdafx.h"

#define OBJECT_X_SIZE 32
#define OBJECT_Y_SIZE 64

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
    HBITMAP Get_Image();
    BITMAP Get_Info();

    void Get_Damage(int);
    void Attack(Object);
};
