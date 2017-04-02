#pragma once
#include "object.h"
#include "player.h"
#define FPS 60
class gimmick : public object {
public:
	int id;//0:clothes 255:hp max up
	int i;
	bool solid;
	bool item;
	
	gimmick(int p_id, manimation p_sprite) :object(1, -2, 0, p_sprite)
	{
		id = p_id;
		i = 0;
		if (id == 1)
			solid = true;
		else
			solid = false;
		if (id == 255)
			item = true;
		else
			item = false;
	}
	void effect(player *milk);
	void put_right(player *milk);
	void put_left(player *milk);

};