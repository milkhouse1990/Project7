#pragma once
#include <atlimage.h>
#include "imageparameter.h"
#include "manimation.h"
#include<list>

class object:public manimation{
public:
	int nHp;
	int mHp;
	int vx;
	double vy;
	
	int invincible;

	//for animation and collision check
	//manimation sprite;


	object(int hp, int p_vx, int p_vy, manimation p_sprite):manimation(p_sprite)
	{
		mHp = nHp = hp;
		vx = p_vx;
		vy = p_vy;
		invincible = 0;
				
	}
//private:
	void motion();
	
	
};


