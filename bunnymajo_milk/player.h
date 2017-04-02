#pragma once
#include "object.h"
extern const int tile;
extern bool WallCheck(int x, int y);
class player: public object {
public:
	int costume;
	int motion;//0:stand 1:move 2:down 3:jump 4:attack
	int speed;
	bool double_jump;
	player(int p_hp, manimation p_sprite) :object(p_hp, 0, 0, p_sprite)
	{
		costume = 0;
		motion = 0;
		speed = 8;
		double_jump = false;
		//timer = 0;
		sprite.ubox = 4;
		sprite.vbox = 0;
		sprite.wbox = tile;
		sprite.hbox = 2 * tile;
	}
	void die();
	bool ground_check();
	bool left_wall_check(int delta_x=0);
	bool right_wall_check(int delta_x=0);
	void turn_left(bool left);
	
};
