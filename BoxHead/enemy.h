#include "head_library.h"
#include "object.h"

class Enemy : public Object
{
private:
public:
	Enemy(int hp, int atk, int sp) : Object(hp, atk, sp) {}
};