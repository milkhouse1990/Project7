#pragma once
#include "object.h"
class npc: public object{
public:
	int id;

	npc(int p_id, manimation p_sprite):object(1, 0, 0, p_sprite)

	{
		id = p_id;

	}
};
