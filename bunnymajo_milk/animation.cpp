#include "animation.h"

void animation::draw(HDC mdc, int px, int py, bool left)
{
	
	if (left)
	{
		CImage image2;
		RotateCimage(&image, &image2);
		image2.Draw(mdc, px, py, xsize, ysize, xsize*frame, 0, xsize, ysize);
		image2.Destroy();
	}
	else
		image.Draw(mdc, px, py, xsize, ysize, xsize*frame, 0, xsize, ysize);

	step++;
	if (step >= period)
	{
		step = 0;
		frame++;
		if (frame >= frames)
			frame = 0;
	}
		
}

void animation::draw(HDC mdc)
{
	//CImage image;
	//Mimage.Load(resdir + "\\" + name + ".png");
	image.Draw(mdc, x-xsize/2+1, y-ysize+1, xsize, ysize, xsize*frame, 0, xsize, ysize);

	step++;
	if (step >= period)
	{
		step = 0;
		frame++;
		if (frame >= frames)
			frame = 0;
	}
}

int animation::get_gx()
{
	int gx = (x - xsize / 2 + 1) / tile;
	return gx;
}

int animation::get_gy()
{
	int gy = (y - ysize + 1) / tile;
	return gy;
}

void animation::change(CString p_name, int p_period, int p_frames)
{
	name = p_name;
	step = 0;
	frame = 0;
	period = p_period;
	frames = p_frames;
	
	image.Load(resdir + "\\" + name + ".png");
	xsize = image.GetWidth() / frames;
	ysize = image.GetHeight();
	
}
