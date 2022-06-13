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

void Tower::Tower_Oparate(Bullet* b)
{

	Bullet* tmp = b;
	Bullet* newnode;

	switch (id)
	{
	case ID_SNIPE_TOWER:
		newnode = (Bullet*)malloc(sizeof(Bullet));

		newnode->Shot_bullet(SNIPER_DAMAGE, way, location);
		newnode->Set_Rlink(NULL);
		newnode->Set_Type(SNIPE_BULLET);

		while (tmp != NULL)
		{
			tmp = tmp->Get_Rlink();
		}

		tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
		break;
	case ID_STD_TOWER:
		newnode = (Bullet*)malloc(sizeof(Bullet));

		newnode->Shot_bullet(PISTOL_DAMAGE, way, location);
		newnode->Set_Rlink(NULL);
		newnode->Set_Type(STD_BULLET);

		while (tmp != NULL)
		{
			tmp = tmp->Get_Rlink();
		}

		tmp->Set_Rlink(newnode); newnode->Set_Llink(tmp);
		break;
	}
}

void Tower::Set_way(int _way)
{
	way = _way;
}

void Tower::Set_id(int _id)
{
	id = _id;
}