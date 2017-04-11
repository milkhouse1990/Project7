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
		_T("����"));
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
		str2 = "��NPC�������ð��ģʽ";
		break;
	case 1:
		str2 = "��ͨ��ƽ̨�ؿ�";
		break;
	case 2:
		str2 = "��...������";
		break;
	case 3:
		str2 = "���ٹ�·׷��ս";
		break;
	case 4:
		str2 = "�����BOSS��";
		break;
	}
	TextOut(mdc, 5*tile, 10*tile-8, str2, str2.GetLength());
	
	vector.draw(mdc,x-tile,y-5+10*index);
	rectangle.draw(mdc,x-tile,y-7);
	//logo.draw(mdc);
}
