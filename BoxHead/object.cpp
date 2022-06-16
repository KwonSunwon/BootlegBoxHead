#include "object.h"

void Object::Set_Image(HBITMAP Bitmap)
{
    image = Bitmap;
    GetObject(image, sizeof(BITMAP), &info);
}

void Object::Set_Info()
{
    GetObject(image, sizeof(BITMAP), &info);
}

int Object::Get_Health()
{
    return health;
}

int Object::Get_Attack()
{
    return attack;
}

int Object::Get_Speed()
{
    return speed;
}

POINT Object::Get_Location()
{
    return location;
}

POINT Object::Get_Position()
{
    POINT pos;

    pos.x = location.x / TILE_SIZE;
    pos.y = location.y / TILE_SIZE;

    return pos;
}

HBITMAP Object::Get_Image()
{
    return image;
}

BITMAP Object::Get_Info()
{
    return info;
}

void Object::draw(HDC hdc)
{
    HDC mdc = CreateCompatibleDC(hdc);
    SelectObject(mdc, image);
    TransparentBlt(hdc, location.x - OBJECT_X_SIZE / 2, location.y - OBJECT_Y_SIZE / 2, OBJECT_X_SIZE, OBJECT_Y_SIZE,
                   mdc, 0, 0, OBJECT_X_SIZE, OBJECT_Y_SIZE, RGB(255, 255, 255));
    DeleteDC(mdc);
}

RECT Object::Get_Rect()
{
    RECT temp = {
        location.x -= (OBJECT_X_SIZE / 2),
        location.y -= (OBJECT_Y_SIZE / 2),
        location.x += (OBJECT_X_SIZE / 2),
        location.y += (OBJECT_Y_SIZE / 2),
    };
    return temp;
}

void Object::Set_Location(POINT _location)
{
    location.x = _location.y * TILE_SIZE;
    location.y = _location.x * TILE_SIZE;
}

void Object::Set_Health(int _health)
{
    health = _health;
}

void Object::Set_Attack(int _attack)
{
    attack = _attack;
}

void Object::Set_Speed(int _speed)
{
    speed = _speed;
}

void Object::Get_Damage(int _damage)
{
    health -= _damage;
}

void Object::Attack(Object _target)
{
    _target.Get_Damage(attack);
}

void Object::Get_Heal(int _heal)
{
    if ((health + _heal) <= MAX_OBJECT_HEALTH)
    {
        health += _heal;
    }

    else
    {
        health = MAX_OBJECT_HEALTH;
    }
}
