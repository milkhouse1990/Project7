#include "gimmick.h"
void gimmick::effect(player *milk)
{
	switch (id)
	{
	case 0:
		break;
	case 1:
		sprite.x += vx;
		if (sprite.damage_check(milk->sprite))
			if (vx > 0)
				put_right(milk);
			else
				put_left(milk);
		else
		{
			milk->sprite.y++;
			if (sprite.damage_check(milk->sprite))
				milk->sprite.x += vx;
			milk->sprite.y--;
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
	milk->sprite.x = sprite.x -sprite.xsize/2+sprite.ubox+sprite.wbox-milk->sprite.ubox+milk->sprite.xsize / 2;
}
void gimmick::put_left(player *milk)
{
	milk->sprite.x = sprite.x - sprite.xsize / 2 -milk->sprite.wbox- milk->sprite.ubox + milk->sprite.xsize / 2;
}