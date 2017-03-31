#include"enemy.h"
void enemy::thinking()
{
	if (timer == 0)
		switch (id)
		{
		case 1:
			timer = 3*60;
			skill = 1;
			break;
		case 10:
			switch (rand() % 4)
			{
			case 0:
				timer = 60;
				skill = 1;
				break;
			case 1:
				timer = 60;
				skill = 2;
				break;
			case 2:
				timer = 60;
				skill = 3;
				break;
			case 3:
				timer = 60;
				skill = 4;
				break;
			}
			break;
		}
		
}
void enemy::UseSkill(list<enemy>* enemy_list)
{
	if (!skill)
		return;
	switch (id)
	{
	case 1:
	{
		if (skill == 1)
		{
			manimation a_bullet(sprite.get_gx(),sprite.get_gy(),"bullet");
			enemy bullet(2, 1, -5, 0, a_bullet);
			enemy_list->push_back(bullet);
			skill = 0;
			break;
		}
		
	}
		
	case 10:
		switch (skill)
		{
		case 1:
			sprite.y -= 0;
			break;
		case 2:
			sprite.y += 0;
			break;
		case 3:
			sprite.x -= 1;
			break;
		case 4:
			sprite.x += 1;
			break;
		}
		break;
	}
}
void enemy::update()
{
	timer--;
}