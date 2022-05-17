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
    Object(int _health, int _attack, int _speed, POINT firstLocation);

    virtual void Move_left() = 0;

    virtual void Move_right() = 0;

    virtual void Move_up() = 0;

    virtual void Move_down() = 0;
};

Object::Object(int _health, int _attack, int _speed, POINT firstLocation) 
{
    location = firstLocation;
    health = _health;
    attack = _attack;
    speed = _speed;
}
