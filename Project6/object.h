#pragma once
#include <atlimage.h>
#include "imageparameter.h"
#include "manimation.h"
#include<list>

class object{
public:
	int nHp;
	int mHp;
	//int x;
	//int y;
	int vx;
	int vy;
	
	bool left;
	int invincible;

	//for animation and collision check
	manimation sprite;


	object(int hp, int p_vx, int p_vy, manimation p_sprite):sprite(p_sprite)
	{
		mHp = nHp = hp;
		//x = gx+sprite.xsize/2-1;
		//y = gy+sprite.ysize-1;
		vx = p_vx;
		vy = p_vy;
		invincible = 0;
		left = false;
		
	}
//private:
	void motion();
	void draw(HDC mdc, int xview, int yview);
	
};


