#include"player.h"
//#include<stdio.h>
void player::die()
{
	
}

bool player::ground_check()
{
	int dx = get_left_border();
	int i;
	for (i=0;i<wbox/tile;i++)
	{
		if (WallCheck(dx, (int)y + 1))
			return true;
		dx += tile;
	}
	if (WallCheck(dx-i*tile + wbox - 1, (int)y + 1))
		return true;
	return false;
}

bool player::left_wall_check(int delta_x)
{
	int dx = get_left_border();
	dx-=delta_x;
	int dy = y;
	int i;
	for (i = 0; i < hbox / tile; i++)
	{
		if (WallCheck(dx, dy))
			return true;
		dy -= tile;
	}
	if (WallCheck(dx, dy+i*tile-hbox+1))
		return true;
	return false;
}

bool player::right_wall_check(int delta_x)
{
	int dx = get_right_border();
	dx+= delta_x;
	int dy = y;
	int i;
	for (i = 0; i < hbox / tile; i++)
	{
		if (WallCheck(dx, dy))
			return true;
		dy -= tile;
	}
	if (WallCheck(dx, dy + i*tile - hbox + 1))
		return true;
	return false;
}

void player::turn_left(bool p_left)
{
	if (left != p_left)
	{
		left = p_left;
		xoffset = -xoffset;
	}
	motion = 1;
	mchange("milk_walk");
}

void player::costume_change(int cos)
{
	switch (cos)
	{
	case 0:
		costume = 0;
		speed = 8;
		motion = 0;
		mchange("rabbit", 0, 1, 0, tile, 2 * tile);
		break;
	case 1:
		costume = 1;
		change("milk_maid");
		break;
	case 5:
		costume = 5;
		speed = 16;
		motion = 0;
		mchange("milk_bunny", 0, 1, 0, tile, 0.5*tile);
		break;
	case 7:
		costume= 7;
		mchange("milk_bathtowel");
		break;
	default:
		break;
	}
}

void player::xphysics()
{
	x += vx;
}
void player::yphysics()
{
	y += vy;
	if (y < 9 * tile&&vy == 0)
	{
		int i = 0;
	}
	vy += g; 
	
}

void player::stop()
{
	vx = 0;
	motion = 0;
}

void player::jump()
{
	vy = -12.0*tile / FPS;// +0.5*g;
}
