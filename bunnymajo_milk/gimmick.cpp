#include "gimmick.h"
void gimmick::effect(player *milk)
{
	switch (id)
	{
	case 0:
		break;
	case 1:
		x += vx;
		if (damage_check(*milk))
			if (vx > 0)
				put_right(milk);
			else
				put_left(milk);
		else
		{
			milk->y++;
			if (damage_check(*milk))
				milk->x += vx;
			milk->y--;
		}
		break;
	case 2://auto recover
		i++;
		if (i >= 0.5*FPS)
		{
			i = 0;
			milk->nHp++;
			if (milk->nHp > milk->mHp)
				milk->nHp = milk->mHp;
		}
		break;
			

	case 255:
		milk->mHp++;
		milk->nHp = milk->mHp;
		break;
	}
}

void gimmick::put_right(player *milk)
{
	milk->x = x -xsize/2 + ubox + wbox - milk->ubox + milk->xsize / 2;
}
void gimmick::put_left(player *milk)
{
	milk->x = x - xsize / 2 -milk->wbox- milk->ubox + milk->xsize / 2;
}