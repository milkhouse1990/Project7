#include "ui.h"

void draw_hp_gauge(HDC mdc, int x, int y, int hp, int mhp)
{
	CImage image;
	image.Load(resdir + "\\life.png");
	
	int j=tile/16;

	int i;
	if (mhp > 32)
		i = 32;
	else
		i = mhp;
	i = i * j * 2;
	image.Draw(mdc, x, y - i - j, 8 * j, i, 0, 0, 8 * j, i);

	i = hp;
	if (i==0)
		image.Draw(mdc, x, y - j, 8 * j, j, 8 * j, 65 * j - 1, 8 * j, j);
	else
	{
		while (i > 32)
		{
			image.Draw(mdc, x, y - 32 * j * 2, 8 * j, 32 * j * 2, 8 * j*(i/32), 65 * j - 1 - 32 * j * 2 + j, 8 * j, 32 * j * 2);
			i -= 32;
		}
		i = i*j * 2;
		image.Draw(mdc, x, y - i, 8 * j, i, 16 * j, 65 * j - 1 - i + j, 8 * j, i);
	}		
	
	image.Destroy();
}
