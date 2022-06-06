#pragma once
#include "stdafx.h"
#include "object.h"

class Enemy : public Object
{
private:
public:

    void Move_left();
    void Move_right();
    void Move_up();
    void Move_down();
};