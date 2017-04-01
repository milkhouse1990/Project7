#pragma once
#include "animation.h"
class manimation :public animation {
public:
	int ubox;
	int vbox;
	int wbox;
	int hbox; //collision box: x:ubox~ubox+wbox-1; y:vbox~vbox-hbox+1;

	manimation(int gx, int gy, CString p_name, int p_period = 0, int p_frames = 1) :animation(gx, gy, p_name, p_period, p_frames)
	{
		ubox = 0;
		vbox = 0;
		wbox = xsize;
		hbox = ysize;
	}
	manimation(const manimation& ma):animation(0,0,ma.name,ma.period,ma.frames)
	{
		x = ma.x;
		y = ma.y;
		ubox = ma.ubox;
		vbox = ma.vbox;
		wbox = ma.wbox;
		hbox = ma.hbox;
	}
	bool damage_check(manimation enemy);
	void drawg(HDC mdc, bool left, int xview, int yview);
	void mchange(CString p_name, int p_period=0, int p_frames=1);
	void mchange(CString p_name, int p_period, int p_frames, int p_ubox, int p_wbox, int p_hbox);
};