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
};

Object::Object(int _health, int _attack, int _speed, POINT firstLocation) {
    location = firstLocation;
    health = _health;
    attack = _attack;
    speed = _speed;
}