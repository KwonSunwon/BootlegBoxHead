#include "head_library.h"

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
    Object(int hp, int atk, int sp);
};

Object::Object(int hp, int atk, int sp) {
    health = hp;
    attack = atk;
    speed = sp;
}