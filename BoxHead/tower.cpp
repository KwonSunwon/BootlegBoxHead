#include "map.h"
#include "player.h"
#include "tower.h"

int Get_distance(POINT a, POINT b)
{
	return fabs(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

int Tower::Get_id() 
{
	return id;
}

void Tower::Heal_target(Player _target)
{
	int distance = Get_distance(_target.Get_Location(), location);

	if (distance < HEAL_DISTANCE)
	{
		_target.Get_Heal(HEAL_AMOUNT);
	}
}

void Tower::Operate(Object _target) 
{
	switch (id)
	{
	case HEAL_TOWER_ID:
		//Heal_target();
		break;
	case SNIPE_TOWER_ID:
		break;
	case BOMB_TOWER_ID:
		break;
	case STD_TOWER_ID:
		break;
	}
}