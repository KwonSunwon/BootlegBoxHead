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

    void Move_left() {}
    void Move_right() {}
    void Move_up() {}
    void Move_down() {}

    void Set_Image(HBITMAP);

    void Set_Info();

    int Get_Health();
    int Get_Attack();
    int Get_Speed();
    POINT Get_Location();
};
