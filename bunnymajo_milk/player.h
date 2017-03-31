#pragma once
#include "object.h"
extern const int tile;
class player: public object {
public:
	int costume;
	int motion;//0:stand 1:move 2:down 3:jump 4:attack
	int speed;
	player(int p_hp, manimation p_sprite) :object(p_hp, 0, 0, p_sprite)
	{
		costume = 0;
		motion = 0;
		speed = 5;
		//timer = 0;
		sprite.ubox = 4;
		sprite.vbox = 0;
		sprite.wbox = tile;
		sprite.hbox = 2 * tile;
	}
	void die();
	
};
