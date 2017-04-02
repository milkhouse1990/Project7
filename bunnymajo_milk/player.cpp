#include"player.h"
//#include<stdio.h>
void player::die()
{
	
}

bool player::ground_check()
{
	int dx = sprite.x - sprite.xoffset - sprite.xsize / 2 + 1 + sprite.ubox;
	int i;
	for (i=0;i<sprite.wbox/tile;i++)
	{
		if (WallCheck(dx, (int)sprite.y + 1))
			return true;
		dx += tile;
	}
	if (WallCheck(dx-i*tile + sprite.wbox - 1, (int)sprite.y + 1))
		return true;
	return false;
}

bool player::left_wall_check(int delta_x)
{
	int temp_x = sprite.x - delta_x;
	int dx = temp_x - sprite.xoffset - sprite.xsize / 2 + 1 + sprite.ubox;
	int dy = sprite.y;
	int i;
	for (i = 0; i < sprite.hbox / tile; i++)
	{
		if (WallCheck(dx, dy))
			return true;
		dy -= tile;
	}
	if (WallCheck(dx, dy+i*tile-sprite.hbox+1))
		return true;
	return false;
}

bool player::right_wall_check(int delta_x)
{
	int temp_x=sprite.x + delta_x;
	int dx = temp_x - sprite.xoffset - sprite.xsize / 2 + sprite.ubox + sprite.wbox;
	int dy = sprite.y;
	int i;
	for (i = 0; i < sprite.hbox / tile; i++)
	{
		if (WallCheck(dx, dy))
			return true;
		dy -= tile;
	}
	if (WallCheck(dx, dy + i*tile - sprite.hbox + 1))
		return true;
	return false;
}

void player::turn_left(bool left)
{
	if (sprite.left != left)
	{
		sprite.left = left;
		sprite.xoffset = -sprite.xoffset;
	}
}
