#include "map.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"

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