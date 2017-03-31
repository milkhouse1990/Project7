#pragma once
#include <list>

#include "object.h"
extern const int tile;
using namespace std;
//enemy
class enemy :public object {
public:
	int id;
	int timer;
	int skill;
	int atk;
	enemy(int p_id,int p_hp, int p_vx, int p_vy, manimation p_sprite) :object(p_hp, p_vx, p_vy, p_sprite)
	{
		id = p_id;
		timer = 0;
		skill = 0;
		atk = 1;
	}
	void thinking();
	void UseSkill(list<enemy>* enemy_list);
	void update();
};