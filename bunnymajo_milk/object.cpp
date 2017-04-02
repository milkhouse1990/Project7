#include "object.h"
#include<atlimage.h>
void object::motion()
{
	sprite.x += vx;
	sprite.y += vy;
}
void object::draw(HDC mdc,int xview,int yview)
{
	sprite.drawg(mdc, xview, yview);
}


