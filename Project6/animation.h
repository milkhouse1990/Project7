#pragma once
#include<atlimage.h>
#include"imageparameter.h"
extern CString resdir;
extern const int tile;
class animation
{
public:
	//location
	int x;
	int y;
	//CImage image;

	CString name;
	int xsize;
	int ysize; //size of one frame: xsize*ysize

	//animation control
	int step;
	int frame;
	int frames;
	int period;

	animation(int gx, int gy, CString p_name, int p_period=0,int p_frames=1)
	{
		change(p_name,p_period,p_frames);		

		x = gx *tile+ xsize / 2 - 1;
		y = gy *tile+ ysize - 1;
	}
	//~animation()
	//{
		//image.Destroy();
	//}
	void draw(HDC mdc, int px,int py,bool left=false);
	void draw(HDC mdc);
	int get_gx();
	int get_gy();
	void change(CString p_name, int p_period = 0, int p_frames = 1);
};