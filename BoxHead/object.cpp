#include "object.h"


void Object::Set_Image(HBITMAP Bitmap) 
{
    image = Bitmap;
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


HBITMAP Object::Get_Image()
{
    return image;
}

BITMAP Object::Get_Info()
{
    return info;
}

void Object::Set_Location(POINT _location ) 
{
    location = _location;
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
