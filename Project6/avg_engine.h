#pragma once
#include<atlimage.h>
#include <fstream>
#include <vector>
#include "animation.h"
extern const int tile;
using namespace std;
class avgengine
{
public:
	CString dialogue;
	CString dialogue_next;
	ifstream file;
	string speaker;
	CString words;
	vector<CString> words0;
	bool wait;
	bool pause;
	bool al;
	int alarm;
	//for debug
	string errmsg;
	bool err;
	//for ui
	animation frame;
	animation icon;
	animation np;
	avgengine() :frame(2,1,"dialogue"), icon(0,1,"icon_milk"), np(17,2,"a", 10, 2)
	{
		dialogue = "";
		dialogue_next = "";
		speaker = "";
		words = "";
		errmsg = "";
		err = false;
		wait = false;
		pause = false;
	}
	void boot(CString title);
	bool step();
	void draw(HDC mdc, int xview, int yview);
	void openfile();
	bool display(HDC mdc);
	void nextpage();
};
	
	
