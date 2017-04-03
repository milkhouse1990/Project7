#include "manimation.h"

bool manimation::damage_check(manimation enemy)
{
	//milk:def enemy:atk
	int pdx = get_left_border();
	int edx = enemy.get_left_border();
	if (pdx <= edx + enemy.wbox - 1)
		if (pdx + wbox - 1 >= edx )
			if (y - vbox >= enemy.y - enemy.vbox - enemy.hbox + 1)
				if (y - vbox - hbox + 1 <= enemy.y - enemy.vbox)
					return true;
	return false;
}

void manimation::drawg(HDC mdc, int xview, int yview)
{
	draw(mdc, x - xoffset - xsize / 2 + 1 - xview, y - ysize + 1 - yview);
}

void manimation::mchange(CString p_name, int p_period, int p_frames)
{
	name = p_name;
	step = 0;
	frame = 0;
	period = p_period;
	frames = p_frames;

	image.Destroy();
	image.Load(resdir + "\\" + name + ".png");
	xsize = image.GetWidth() / frames;
	ysize = image.GetHeight();
	
	//ubox = 0;
	//vbox = 0;
	//wbox = xsize;
	//hbox = ysize;
}
void manimation::mchange(CString p_name, int p_period, int p_frames, int p_xoffset, int p_wbox, int p_hbox)
{
	mchange(p_name, p_period, p_frames);
	xoffset = p_xoffset;
	if (left)
		xoffset = -xoffset;
	//ubox = (xsize - wbox) / 2;
	wbox = p_wbox;
	hbox = p_hbox;
}

int manimation::get_gx()
{
	int gx = get_left_border() / tile;
	return gx;
}
int manimation::get_left_border()
{
	int ubox= (xsize - wbox) / 2;
	int dx = x - xoffset - xsize / 2 + 1 + ubox;
	//dx=x-xoffset-wbox/2+1
	return dx;
}

int manimation::get_right_border()
{
	int dx = x - xoffset + wbox / 2;
	return dx;
}
