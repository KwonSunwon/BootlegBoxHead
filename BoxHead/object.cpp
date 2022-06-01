#include "object.h"


void Object::Set_Image(HBITMAP Bitmap) {
    image = Bitmap;
}

void Object::Set_Info() {
    GetObject(image, sizeof(BITMAP), &info);
}

int Object::Get_Health() {
    return health;
}

int Object::Get_Attack() {
    return attack;
}

int Object::Get_Speed() {
    return speed;
}

POINT Object::Get_Location() {
    return location;
}