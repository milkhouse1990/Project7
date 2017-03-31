#include "manimation.h"

bool manimation::damage_check(manimation enemy)
{
	//milk:def enemy:atk
	int pdx = x - xsize / 2 + 1;
	int edx = enemy.x - enemy.xsize / 2 + 1;
	if (pdx + ubox <= edx + enemy.ubox + enemy.wbox - 1)
		if (pdx + ubox + wbox - 1 >= edx + enemy.ubox)
			if (y - vbox >= enemy.y - enemy.vbox - enemy.hbox + 1)
				if (y - vbox - hbox + 1 <= enemy.y - enemy.vbox)
					return true;
	return false;
}

void manimation::drawg(HDC mdc, bool left, int xview, int yview)
{
	draw(mdc, x - xsize / 2 + 1 - xview, y - ysize + 1 - yview,left);
}

void manimation::mchange(CString p_name, int p_period, int p_frames)
{
	name = p_name;
	step = 0;
	frame = 0;
	period = p_period;
	frames = p_frames;

	CImage image;
	image.Load(resdir + "\\" + name + ".png");
	xsize = image.GetWidth() / frames;
	ysize = image.GetHeight();
	image.Destroy();

	ubox = 0;
	vbox = 0;
	wbox = xsize;
	hbox = ysize;
}
void manimation::mchange(CString p_name, int p_period, int p_frames, int p_ubox, int p_wbox, int p_hbox)
{
	mchange(p_name, p_period, p_frames);
	ubox = p_ubox;
	wbox = p_wbox;
	hbox = p_hbox;
}
