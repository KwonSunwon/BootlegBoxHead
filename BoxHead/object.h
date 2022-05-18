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

    void Move_left() {}
    void Move_right() {}
    void Move_up() {}
    void Move_down() {}

    void Set_Image(HBITMAP);
};

Object::Object(int _health, int _attack, int _speed, POINT firstLocation)
{
    health = _health;
    attack = _attack;
    speed = _speed;
    image = NULL;
    location = firstLocation;
}

void Object::Set_Image(HBITMAP Bitmap) {
    image = Bitmap;
}