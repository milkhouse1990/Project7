#pragma once
#include "player.h"
class event{
public:
	int id;
	int x;
	int y;
public:
	//id0
	event(int p_id, int p_x, int p_y)
	{
		id = p_id;
		x = p_x;
		y = p_y;
	}
	//ac
	void action(player *milk);
};