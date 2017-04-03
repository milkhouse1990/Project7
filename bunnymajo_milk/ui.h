#pragma once
#include <atlimage.h>
extern CString resdir;
extern const int tile;
//x,y:left_bottom ordicate
void draw_hp_gauge(HDC mdc, int x, int y, int hp, int mhp);
