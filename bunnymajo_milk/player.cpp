#include"player.h"
//#include<stdio.h>
void player::die()
{
	
}

bool player::ground_check()
{
	int dx = x - xoffset - xsize / 2 + 1 + ubox;
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
	int temp_x = x - delta_x;
	int dx = temp_x - xoffset - xsize / 2 + 1 + ubox;
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
	int temp_x=x + delta_x;
	int dx = temp_x - xoffset - xsize / 2 + ubox + wbox;
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

void player::physics()
{
	y += vy;
	vy += 36.0 * tile / FPS / FPS;
}
