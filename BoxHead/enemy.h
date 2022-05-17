#include "head_library.h"
#include "object.h"

class Enemy : public Object
{
private:
public:
	Enemy(int _health, int _attack, int _speed, POINT firstLocation) : Object(_health, _attack, _speed, firstLocation) {}
};