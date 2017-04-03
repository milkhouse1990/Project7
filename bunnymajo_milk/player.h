#pragma once
#include "object.h"
#include "main.h"
extern const int tile;
extern const int FPS;
extern bool WallCheck(int x, int y);
class player: public object {
public:
	//0:majo 1:maid 5:bunny 6:dress 7:bathtowel
	int costume;
	int motion;//0:stand 1:move 2:down 3:jump 4:attack
	int speed;
	double g;
	bool double_jump;
	player(int p_hp, manimation p_sprite) :object(p_hp, 0, 0, p_sprite)
	{
		costume_change(0);
		double_jump = false;
		//timer = 0;
		//ubox = 4;
		vbox = 0;
		g= 36.0 * tile / FPS / FPS;
	}
	void die();
	bool ground_check();
	bool left_wall_check(int delta_x=0);
	bool right_wall_check(int delta_x=0);
	void costume_change(int cos);
	void xphysics();
	void yphysics();
	//action
	void turn_left(bool left);
	void stop();
	void jump();
};
