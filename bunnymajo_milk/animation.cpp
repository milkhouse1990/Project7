#include "animation.h"

void animation::draw(HDC mdc, int px, int py, bool left)
{
	CImage image,image2;
	image.Load(resdir+"\\"+name+".png");
	if (left)
	{
		RotateCimage(&image, &image2);
	}
	else
		image2 = image;
	//image.Destroy();
	
	//HDC hdc = image.GetDC();
	//SelectObject(mdc, image.a);
	//HBITMAP h_bmp = image.Detach();
	//SelectObject(mdc, &image);
	//image.Attach(h_bmp);
	//HDC hdc = image.GetDC();
	image2.Draw(mdc, px, py, xsize, ysize, xsize*frame, 0, xsize, ysize);
	//image.ReleaseDC();
	//DeleteObject(&image);
	image.Destroy();
	image2.Destroy();

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
	CImage image;
	image.Load(resdir + "\\" + name + ".png");
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
	CImage image;
	name = p_name;
	step = 0;
	frame = 0;
	period = p_period;
	frames = p_frames;

	image.Load(resdir + "\\" + name + ".png");
	xsize = image.GetWidth() / frames;
	ysize = image.GetHeight();
	image.Destroy();
}
