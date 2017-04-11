#include "menu.h"

void menu::plus()
{
	index++;
	if (index == items)
		index = 0;
}

void menu::minus()
{
	index--;
	if (index == -1)
		index = items - 1;
}

void menu::draw(HDC mdc)
{
	CString str;
	CString str2;
	/*HFONT efont = CreateFont(13,                                                  //   nHeight  
		8,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		FW_NORMAL,                                   //   nWeight  
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("宋体"));
	SelectObject(mdc, efont);*/
	for (int i = 0; i < items; i++)
	{
		switch(i)
		{
		case 0:
			str = "ADVENTURE";
			break;
		case 1:
			str = "PLATFORM";
			break;
		case 2:
			str = "RABBIT";
			break;
		case 3:
			str = "HIGHWAY";
			break;
		case 4:
			str = "BOSS";
			break;
		}
	
		TextOut(mdc, x, y + i * 10, str, str.GetLength());
	}
	switch (index)
	{
	case 0:
		str2 = "和NPC聊聊天的冒险模式";
		break;
	case 1:
		str2 = "普通的平台关卡";
		break;
	case 2:
		str2 = "是...是兔子";
		break;
	case 3:
		str2 = "高速公路追逐战";
		break;
	case 4:
		str2 = "来打个BOSS吧";
		break;
	}
	TextOut(mdc, 5*tile, 10*tile-8, str2, str2.GetLength());
	
	vector.draw(mdc,x-tile,y-5+10*index);
	rectangle.draw(mdc,x-tile,y-7);
	//logo.draw(mdc);
}
