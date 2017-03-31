#pragma once
#include <atlimage.h>

#include "animation.h"
#include <string.h>
extern const int tile;
using namespace std;
class menu {
public:
	int x;
	int y;
	int index;
	int items;
	bool vertical;
	animation vector;
	animation rectangle;
	animation logo;
public:
	menu(int p_x, int p_y, int p_items, bool p_vertical = true) :vector(7,5,"vector", 10, 2),rectangle(7,5,"rectangle"),logo(0,0,"logo")
	{
		x = p_x;
		y = p_y;
		index = 0;
		items = p_items;
		vertical = p_vertical;
	}
	void plus();
	void minus();
	void draw(HDC mdc);
};
