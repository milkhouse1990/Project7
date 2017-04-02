//#include "stdafx.h"
#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib,"winmm.lib" )

#include<Windows.h>
#include <stdio.h>
#include<string.h>
#include<dinput.h>
#include<atlimage.h>
#include<list>
#include<vector>
#include"main.h"
#include"animation.h"

#include"object.h"
#include"npc.h"
#include"enemy.h"
#include"player.h"
#include"gimmick.h"
#include "event.h"

#include"menu.h"

#include"avg_engine.h"
#include"keyconfig.h"

using namespace std;

#define DINPUT_BUFFERSIZE 16

const int xscreen = 320, yscreen = 180;
const int tile = 16;

//全局变量声明

HINSTANCE hInst;
HBITMAP bg, game,rabbit,fullmap,bmp;
HDC     hdc, mdc, bufdc,mapdc;
HWND    hWnd;
DWORD timeBegin = timeGetTime();
ULONGLONG   tPre, tNow,tStart,n=0;
int     f, txtNum;
bool    attack, over, act, transform = false, trans_finish = false, boss_flag = false, selector;
manimation a_milk(0,0,"rabbit");
player milk(16,a_milk);
char    text[5][100];
//int num=0, num2=0,x1=0;
int roll;

avgengine avg;
//CString dialogue(""),dialogue_next("");

list<object> milk_listobj;
list<object>::iterator loi, loi2;

list<event> event_list;
list<event>::iterator levi;

list<enemy> enemy_list;
list<enemy>::iterator lei;

list<npc> npc_listnpc;
list<npc>::iterator lni;

list<gimmick>gimmick_list;
list<gimmick>::iterator lgi;

int rows, cols;

//keyconfig
int BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_L, BUTTON_R, BUTTON_SELECT, BUTTON_START;

//input
LPDIRECTINPUTDEVICE8 g_pKeyboardDevice = NULL;
LPDIRECTINPUT8 g_pDirectInput = NULL;
BYTE g_pKeyStateBuffer[256];
DIDEVICEOBJECTDATA didod[DINPUT_BUFFERSIZE];
DWORD dwElements;

//VIEW
int xview = 0;

//map
vector<int> mapIndex;
int scene = 3;

//menu
menu stage_select(8*tile,6*tile,5);

//全局函数声明  
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void				input();
void				update();
void                paint();

bool WallCheck(int x, int y);
void LoadMap(HDC hdc,HDC bufdc,HDC mapdc);

//计算FPS
float GetFPS()
{
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数+1
	currentTime = timeGetTime()*0.001f;//获取系统时间,并转化成秒为单位
									   //经过了一秒，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime >= 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
	}
	return fps;
}

//input
BOOL Device_Read(IDirectInputDevice8 *pDIDevice, DIDEVICEOBJECTDATA *didod,DWORD *dwElements,BYTE *diks)
{
	DWORD dwElements2;
	HRESULT hr;
	while (true)
	{
		pDIDevice->Poll();              // 轮询设备  
		pDIDevice->Acquire();          // 获取设备的控制权  
		if (SUCCEEDED(hr = pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),didod,&dwElements2,0))&&SUCCEEDED(hr=pDIDevice->GetDeviceState(256,diks)))
		//if (SUCCEEDED(hr = pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements2, 0)))
		//if (SUCCEEDED(hr = pDIDevice->GetDeviceState(256, diks))) 
			break;
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) return FALSE;
		if (FAILED(pDIDevice->Acquire())) return FALSE;
	}
	*dwElements = dwElements2;
	//didod = didod2;
	/*for (int i = 0; i < dwElements2; i++)
	{
		
		if (didod2[i].dwOfs == DIK_J)
			if (didod2[i].dwData & 0x80)
			{
				object *new_bullet = new object(1, milk.sprite.x, milk.sprite.y, 10, 0, "bullet", 64);
				milk_listobj.push_back(*new_bullet);
			}
	}*/
	
	return TRUE;
}

void MyPaint2()
{
	SelectObject(bufdc, bg);
	BitBlt(mdc, 0, 0, xscreen, yscreen, bufdc, 0, 0, SRCCOPY);
	//FPS
	float fp = GetFPS();
	char str[100];
	sprintf_s(str, "FPS: %f", fp);
	TextOut(mdc, 0, 0, str, strlen(str));
	//display
	//SelectObject(mdc, bmp);
	BitBlt(hdc, 0, 0, xscreen, yscreen, mdc, 0, 0, SRCCOPY);
}

//****WinMain函数，程序入口点函数**************************************  
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//初始化    
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//消息循环  
	GetMessage(&msg, NULL, NULL, NULL);            //初始化msg      
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			input();
			update();
			paint();
			while (timeGetTime() - timeBegin < 15)
			{
			}
			timeBegin = timeGetTime();
		}
	}
	g_pKeyboardDevice->Unacquire();
	g_pKeyboardDevice->Release();
	return msg.wParam;
}

//***设计一个窗口类，类似填空题，使用窗口结构体*************************  
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "canvas";
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

//****初始化函数************************************  
//加载位图并设定各种初始值   
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//input
	//创建DirectInput设备  
	DirectInput8Create(hInstance, 0x0800, IID_IDirectInput8, (void**)&g_pDirectInput, NULL);
	g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, NULL);
	//设置数据格式和协作级别  
	g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//设定缓冲区大小
	DIPROPDWORD property;
	property.diph.dwSize = sizeof(DIPROPDWORD);
	property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	property.diph.dwObj = 0;
	property.diph.dwHow = DIPH_DEVICE;
	property.dwData = DINPUT_BUFFERSIZE;
	g_pKeyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &property.diph);
	//获取控制权  
	g_pKeyboardDevice->Acquire();

	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Test", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 0, 0, xscreen*2, yscreen*2+GetSystemMetrics(SM_CYCAPTION), true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	mapdc = CreateCompatibleDC(hdc);

	//scene = 2;
	LoadMap(hdc, bufdc, mapdc);
	bmp = CreateCompatibleBitmap(hdc, xscreen, yscreen);
	SelectObject(mdc, bmp);
	bg = (HBITMAP)LoadImage(NULL, "Res\\bg.bmp", IMAGE_BITMAP, xscreen, yscreen, LR_LOADFROMFILE);

	SetBkMode(mdc, TRANSPARENT);    //设置TextOut背景透明 

	//act = true;
	input();
	update();
	paint();
	//ReleaseDC(hWnd, hdc);

	//KEY CONFIG
	keyconfig kc;
	while (!kc.load_config(&BUTTON_UP, &BUTTON_DOWN, &BUTTON_LEFT, &BUTTON_RIGHT, &BUTTON_A, &BUTTON_B, &BUTTON_X, &BUTTON_Y, &BUTTON_L, &BUTTON_R, &BUTTON_SELECT, &BUTTON_START))
	{
		kc.restore_config();
	}

	return TRUE;
}


void input()
{

	//贴上背景图  
	SelectObject(bufdc, bg);
	BitBlt(mdc, 0, 0, xscreen, yscreen, bufdc, 0, 0, SRCCOPY);
	
	//-----------------------------------------INPUT PROCESS------------------------------
	//input read
	Device_Read(g_pKeyboardDevice, didod, &dwElements, g_pKeyStateBuffer);
	//press control
	for (DWORD i = 0; i < dwElements; i++)
	{
		if (act)
		{
			//act mode
			{
				if (didod[i].dwData & 0x80)
				{
					if (didod[i].dwOfs == BUTTON_X)
					{
						if (milk.costume > 0 && milk.costume < 5)
						{
							milk.costume = 0;
							milk.sprite.change("rabbit");
							trans_finish = true;
						}
					}
					else if (didod[i].dwOfs == BUTTON_Y)
					{
						if (!milk.costume)
						{
							if (!transform)
							{
								manimation a_new_bullet(milk.sprite.get_gx() + 1, milk.sprite.get_gy() + 1, "bullet");
								object new_bullet(1, 10 - 20 * milk.sprite.left, 0, a_new_bullet);
								new_bullet.sprite.left = milk.sprite.left;
								milk_listobj.push_back(new_bullet);
							}
						}
					}
					else if (didod[i].dwOfs == BUTTON_B)
					{
						if (!transform)
						{
							if (milk.ground_check())
							{
								if (milk.motion!=2)
								milk.vy = -12.0*tile/FPS;
							}
							else if (!milk.double_jump)
							{
								milk.vy = -12.0*tile / FPS;
								milk.double_jump = true;
							}
						}
					}
					else if (didod[i].dwOfs == BUTTON_A)
					{
						if (!transform)
						{
							if (milk.motion == 2)
							{
								milk.costume = 5;
								milk.speed = 15;
								milk.motion = 0;
								milk.sprite.mchange("milk_bunny", 0, 1, 4, tile, 0.5*tile);
								manimation a_clothes(milk.sprite.get_gx(), milk.sprite.get_gy(), "bra");
								gimmick clothes(0, a_clothes);
								gimmick_list.push_back(clothes);
							}
							else if (milk.costume == 5)
							{
								//clothes check
								for (lgi = gimmick_list.begin(); lgi != gimmick_list.end();)
								{
									if (lgi->id == 0)
									{
										if (milk.sprite.damage_check(lgi->sprite))
										{
											if (WallCheck(milk.sprite.get_left_border() + milk.sprite.ubox, milk.sprite.y - milk.sprite.hbox) || WallCheck(milk.sprite.x - milk.sprite.xsize / 2 + milk.sprite.wbox, milk.sprite.y - milk.sprite.hbox))
												break;
											else
											{
												milk.costume = 0;
												milk.speed = 10;
												milk.motion = 0;
												milk.sprite.mchange("rabbit", 0, 1, 4, tile, 2 * tile);
												lgi = gimmick_list.erase(lgi);
											}
											break;
										}
										break;
									}
									else
										lgi++;
								}
							}
						}
					}
					else if (didod[i].dwOfs == BUTTON_UP)
					{
						if (!transform)
						{
							//npc check
							for (lni = npc_listnpc.begin(); lni != npc_listnpc.end(); ++lni)
							{
								if (milk.sprite.damage_check(lni->sprite))
								{
									act = false;
									CString title;
									title.Format("npc%d", lni->id);
									avg.boot(title);
									break;
								}
							}
						}
						else
						{
							milk.costume = 1;
							milk.sprite.change("milk_maid");
							trans_finish = true;
						}
					}
					else if (didod[i].dwOfs == BUTTON_DOWN)
					{
						if (transform)
						{
							//milk.costume = 2;
							//trans_finish = true;
						}
						else
						{
							if (milk.ground_check())
								if (milk.costume != 5)
							{
									if (milk.sprite.left)
									{
										if (!milk.left_wall_check(tile))
										{
											milk.motion = 2;
											milk.sprite.mchange("milk_down", 0, 1, -0.5*tile, 2 * tile, tile);
										}
									}
									else
										if (!milk.right_wall_check(tile))
										{
											milk.motion = 2;
											milk.sprite.mchange("milk_down", 0, 1, -0.5*tile, 2 * tile, tile);
										}
							}
						}
					}
					else if (didod[i].dwOfs == BUTTON_LEFT)
					{
						if (transform)
						{
							//milk.costume = 3;
							//trans_finish = true;
						}
					}
					else if (didod[i].dwOfs == BUTTON_RIGHT)
					{
						if (transform)
						{
							//milk.costume = 4;
							//trans_finish = true;
						}
					}
				}
				else
				{
					if (didod[i].dwOfs == BUTTON_DOWN)
					{
						if (transform)
						{

						}
						else
						{
							if (milk.costume != 5)
							{
								milk.motion = 0;
								milk.sprite.mchange("rabbit", 0, 1, 0, tile, 2 * tile);
							}
						}
					}
					else if (didod[i].dwOfs == BUTTON_B)
					{
						if (!transform)
							if (milk.vy < 0)
								milk.vy = 0;
					}
				}
			}
		}
		else
		{
			//avg mode
			if (selector)
			{
				//select item
				if (didod[i].dwData & 0x80)
					if (didod[i].dwOfs == BUTTON_UP)
					{
						if (stage_select.vertical)
							stage_select.minus();
					}
					else if (didod[i].dwOfs == BUTTON_DOWN)
					{
						if (stage_select.vertical)
							stage_select.plus();
					}
					else if (didod[i].dwOfs == BUTTON_A)
					{
						scene = stage_select.index + 1;
						LoadMap(mdc, bufdc, mapdc);
					}
			}
			else
			{
				//next page
				if (didod[i].dwOfs == BUTTON_A)
					if (didod[i].dwData & 0x80)
					{
						avg.pause = false;
						avg.err = false;
						//avg.nextpage();
					}
			}
		}
	}
	//pressing control
	if (act)
	{
		//act mode
		//transform waiting check
		if (g_pKeyStateBuffer[BUTTON_X] & 0x80)
		{
			if (milk.motion == 2)
				trans_finish = true;
			else if (!trans_finish)
				transform = true;
			else
				transform = false;
		}
		else
		{
			transform = false;
			trans_finish = false;
		}
		if (transform)
		{
			//transform control
		}
		else
		{
			//action control
			if (g_pKeyStateBuffer[BUTTON_UP] & 0x80)
			{

			}
			if (g_pKeyStateBuffer[BUTTON_DOWN] & 0x80)
			{

			}
			if (g_pKeyStateBuffer[BUTTON_LEFT] & 0x80)
			{
				if (milk.motion != 2)
				{
					milk.turn_left(true);
					milk.sprite.x -= milk.speed*tile/FPS;
					while (milk.left_wall_check())
						milk.sprite.x++;	

					for (lgi = gimmick_list.begin(); lgi != gimmick_list.end(); lgi++)
					{
						if (lgi->solid)
							if (milk.sprite.damage_check(lgi->sprite))
								lgi->put_right(&milk);
					}
					//milk.name = "rabbit2";
				}
			}
			else if (g_pKeyStateBuffer[BUTTON_RIGHT] & 0x80)
			{
				if (milk.motion != 2)
				{
					milk.turn_left(false);
					milk.sprite.x += milk.speed*tile/FPS;
					while (milk.right_wall_check())
						milk.sprite.x--;			

					for (lgi = gimmick_list.begin(); lgi != gimmick_list.end(); lgi++)
					{
						if (lgi->solid)
							if (milk.sprite.damage_check(lgi->sprite))
								lgi->put_left(&milk);
					}
				}
			}
			//else
			//milk.name = "rabbit";
		}
	}
}

void update()
{
	if (act)
	{
		if (transform)
		{

		}
		else
		{
			//-------------------------------------------------milk PHASE
			//milk status update
			if (milk.invincible > 0)
				milk.invincible--;
			//milk move
			//milk fall
			milk.sprite.y += milk.vy;
			milk.vy += 36.0 * tile / FPS / FPS;
			//地面检测
			if (milk.ground_check())
			{
				milk.sprite.y = ((int)milk.sprite.y + 1) / tile * tile - 1;
				milk.vy = 0;
				milk.double_jump = false;
			}

				
			for (lgi = gimmick_list.begin(); lgi != gimmick_list.end(); lgi++)
			{
				if (lgi->solid)
				{
					milk.sprite.y++;
					if (lgi->sprite.damage_check(milk.sprite))
					{
						milk.sprite.y = lgi->sprite.y - lgi->sprite.hbox;
						milk.vy = 0;
					}
					else
						milk.sprite.y--;
					lgi->effect(&milk);
				}
			}

			//milk bullet phase
			for (loi = milk_listobj.begin(); loi != milk_listobj.end();)
			{
				//milk bullet behavior
				loi->motion();
				if (loi->sprite.x<xview || loi->sprite.x>xview + xscreen)
				{
					loi = milk_listobj.erase(loi);
					continue;
				}

				//enemy_damage_check
				for (lei = enemy_list.begin(); lei != enemy_list.end();)
				{
					if (lei->id < 100)
						if (lei->sprite.damage_check(loi->sprite))
						{
							lei->nHp -= 1;
							if (lei->nHp == 0)
								lei = enemy_list.erase(lei);
							else
								lei++;
						}
						else
							lei++;
					else
						lei++;

				}

				loi++;

			}

			//event check
			for (levi = event_list.begin(); levi != event_list.end(); levi++)
			{
				if (milk.sprite.x >= levi->x-1 && milk.sprite.x <= levi->x + tile)
				{
					switch (levi->id)
					{
					case 0:
						milk.sprite.mchange("rabbit");
						boss_flag = false;
						scene = 0;
						LoadMap(hdc, bufdc, mapdc);
						break;
					case 1:
						if (milk.costume == 7)
						{
							milk.costume = 0;
							milk.sprite.mchange("rabbit");
						}
						break;
					case 2:
						if (milk.costume == 0)
						{
							milk.costume = 7;
							milk.sprite.mchange("milk_bathtowel");
						}
						break;
					case 3:
						scene = 6;
						LoadMap(hdc, bufdc, mapdc);
						break;
					}


					//LoadMap(2, hdc, bufdc, mapdc);
					break;
				}
			}


			//-----------------------------------------------ENEMY PHASE
			for (lei = enemy_list.begin(); lei != enemy_list.end(); )
			{
				//enemy behavior
				lei->thinking();
				lei->UseSkill(&enemy_list);
				lei->update();
				lei->motion();
				if (lei->sprite.x<xview || lei->sprite.x>xview + xscreen)
				{
					lei = enemy_list.erase(lei);
					continue;
				}

				//milk_damage_check
				if (milk.invincible == 0)
				{
					if (milk.sprite.damage_check(lei->sprite))
					{
						milk.nHp -= lei->atk;
						if (milk.nHp <= 0)
						{
							milk.nHp = 0;
							milk.die();
							LoadMap(hdc, bufdc, mapdc);
							break;
						}
						milk.sprite.x -= 64 * (lei->sprite.x - milk.sprite.x >= 0 ? 1 : -1);
						milk.invincible = 60;
					}
				}

				lei++;
			}
			//----------------------------------------------GIMMICK PHASE
			for (lgi = gimmick_list.begin(); lgi != gimmick_list.end();)
			{
				if (lgi->sprite.x < 0)
				{
					lgi = gimmick_list.erase(lgi);
					continue;
				}
				if (!lgi->solid)
					if (milk.sprite.damage_check(lgi->sprite))
					{
						lgi->effect(&milk);
						if (lgi->item)
						{
							lgi = gimmick_list.erase(lgi);
							continue;
						}
					}
				lgi++;

			}


		}
	}
	else
	{
		if (!selector)
			act = avg.step();
		//act = false;//紧急处理
		//act = avg.display(mdc);	
	}
}
void paint()
{
	char str[100];
	//---------------------------------------------------DRAW
	//camera
	if (roll == 0)
	{
		if (milk.sprite.x - xview > xscreen / 2 && cols * tile - milk.sprite.x > xscreen / 2)
			xview = milk.sprite.x - xscreen / 2;
		if (milk.sprite.x - xview < xscreen / 2 && milk.sprite.x > xscreen / 2)
			xview = milk.sprite.x - xscreen / 2;
		//bounder check
		if (milk.sprite.x < 0)
			milk.sprite.x=0;
	}
	
	if (milk.sprite.y > yscreen)
	{
		milk.die();
		LoadMap(hdc, bufdc, mapdc);
	}
	if (roll==1)
		if (milk.sprite.x < xview)
		{
			milk.die();
			LoadMap(hdc, bufdc, mapdc);
		}

	CImage image;
	//npc draw
	for (lni = npc_listnpc.begin(); lni != npc_listnpc.end(); ++lni)
	{
		lni->draw(mdc,xview, 0);

		if (milk.sprite.damage_check(lni->sprite))
		{
			if (act)
			{
				image.Load(resdir+"\\up.png");
				image.Draw(mdc, milk.sprite.x , milk.sprite.y - 4*tile, tile, tile, 0, 0, tile, tile);
				image.Destroy();
			}
		}
	}
	//enemy draw
	for (lei = enemy_list.begin(); lei != enemy_list.end(); ++lei)
	{
		lei->draw(mdc, xview, 0);
	}
	
	//milk draw
	if (scene>0)
		if (!(milk.invincible % 2))
			milk.draw(mdc, xview, 0);
	
	//milk bullet
	for (loi = milk_listobj.begin(); loi != milk_listobj.end(); ++loi)
	{
		loi->draw(mdc, xview, 0);
	}
	
	//environment draw
	for (lgi = gimmick_list.begin(); lgi != gimmick_list.end(); ++lgi)
	{
		lgi->draw(mdc, xview, 0);

		if (lgi->id == 0)
			if (act)
			{
				if (milk.sprite.damage_check(lgi->sprite))
				{
					static animation a_button(milk.sprite.get_gx(), milk.sprite.get_gy(),"a",10,2);
					a_button.draw(mdc);
				}
			}
	}
	//frontground draw
	if (scene == 6)
	{
		animation a_shelter(8,6,"shelter");
		a_shelter.draw(mdc);
		animation a_shelter2(10, 6, "shelter");
		a_shelter2.draw(mdc);
	}
	//transform select draw
	if (transform)
	{
		static animation a_transform_select(0, 0, "transform_select", 10, 2);
		a_transform_select.draw(mdc, milk.sprite.x - a_transform_select.xsize / 2 + 1, milk.sprite.y - milk.sprite.ysize / 2 - a_transform_select.ysize / 2 + 1);
	}
		
	//UI
	if (!act && selector)
		//menu
		stage_select.draw(mdc);
	else
	{
		//CImage image;
		////animation a_face("face");
		//a_face.draw(mdc, 0, 720 - 256);
		//hp
		image.Load(resdir+"\\life.png");
		int i,j;
		j = tile / 16;
		i = milk.mHp;
		i = i * j*2;
		image.Draw(mdc, 0, yscreen - tile*4 - i- j, 8*j, i, 0, 0, 8*j, i);
		i = milk.nHp;
		i = i * j*2;
		image.Draw(mdc, 0, yscreen-tile*4 - i, 8*j, i, 8*j, 65*j -1- i+j, 8*j, i);
		image.Destroy();

		//boss hp
		if (boss_flag)
		{
			image.Load(resdir + "\\life.png");
			i = 32;
			i = i * j * 2;
			image.Draw(mdc, xscreen-tile, yscreen - tile * 4 - i - j, 8 * j, i, 0, 0, 8 * j, i);
			if (enemy_list.size() != 0)
			{
				i = enemy_list.begin()->nHp;

				if (i > 32)
				{
					image.Draw(mdc, xscreen - tile, yscreen - tile * 4 - 32 * j * 2, 8 * j, 32 * j * 2, 8 * j, 65 * j - 1 - 32 * j * 2 + j, 8 * j, 32 * j * 2);
					i -= 32;
					i = i*j * 2;
					image.Draw(mdc, xscreen - tile, yscreen - tile * 4 - i, 8 * j, i, 16 * j, 65 * j - 1 - i + j, 8 * j, i);
				}
				else
				{
					i = i * j * 2;
					image.Draw(mdc, xscreen - tile, yscreen - tile * 4 - i, 8 * j, i, 8 * j, 65 * j - 1 - i + j, 8 * j, i);
				}
			}
			else
				image.Draw(mdc, xscreen - tile, yscreen - tile * 4 - j, 8 * j, j, 8 * j, 65 * j-1, 8 * j, j);
				
			image.Destroy();
		}
		//dialogue draw
		if (!act)
		{
			avg.draw(mdc, xview, 0);
		}
	}
	if (scene>0)
	//map
	//SelectObject(mapdc, fullmap);
		BitBlt(mdc, 0, 0, xscreen, yscreen, mapdc, xview, 0, SRCAND);
	
	//FPS
	float fp = GetFPS();
	sprintf_s(str, "FPS: %f", fp);
	TextOut(mdc, 0, 0, str, strlen(str));
	//display
	//SelectObject(mdc, bmp);
	StretchBlt(hdc, 0, 0, xscreen*2, yscreen*2, mdc, 0, 0, xscreen, yscreen, SRCCOPY);
	}

bool WallCheck(int x, int y)
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (mapIndex[y / tile * cols + x / tile] != 0)
		return 1;
	else
		return 0;
}

//****消息处理函数***********************************  
//   
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	switch (message)
	{
	case WM_KEYDOWN:                //键盘消息  
		if (wParam == VK_ESCAPE)       //按下Esc键  
			PostQuitMessage(0);
		
		break;
	case WM_LBUTTONDOWN:            //鼠标左键消息  
		if (!attack)
		{
			x = LOWORD(lParam);     //X坐标  
			y = HIWORD(lParam);     //Y坐标  

			if (x >= 500 && x <= 550 && y >= 350 && y <= 400)
				attack = true;
		}
		break;
	case WM_DESTROY:                //窗口结束消息  
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteDC(mapdc);
		DeleteObject(bg);
		DeleteObject(game);
		DeleteObject(fullmap);

		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);
		break;
	default:                        //默认消息  
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//load map
void LoadMap(HDC hdc,HDC bufdc,HDC mapdc)
{
	if (scene > 0)
	{
		//map
		mapIndex.clear();
		switch (scene)
		{
		case 1:
			rows = 12;
			cols = 20;
			mapIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		case 2:
			rows = 12;
			cols = 40;
			mapIndex = {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		case 5:
			rows = 12;
			cols = 20;
			mapIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		case 3:
			rows = 12;
			cols = 20;
			mapIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
				0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		case 4:
			rows = 12;
			cols = 20;
			mapIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		case 6:
			rows = 12;
			cols = 20;
			mapIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			break;
		}
		fullmap = CreateCompatibleBitmap(hdc, cols * tile, rows * tile); //先建立fullmap为空白的位图，将其宽与高分别为“行数*图块宽”与“列数*图块高”。  
		SelectObject(mapdc, fullmap);

		HBITMAP map[3];
		char filename[20] = "";
		int rowNum, colNum;
		int i, x, y;

		//加载各块位图  
		for (i = 0; i<3; i++)  //利用循环转换图文文件名，取出各个图块存与“map[i]”中。图块文件名为“map0.bmp”和“map1.bmp”等。  
		{
			sprintf_s(filename, "Res\\map%d.bmp", i);
			map[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, tile, tile, LR_LOADFROMFILE);
		}

		//按照mapIndex数组中的定义取出对应图块，进行地图拼接   
		for (i = 0; i<rows*cols; i++)
		{
			SelectObject(bufdc, map[mapIndex[i]]);  //根据mapIndex[i]中的代号选取对应的图块到bufdc中。代号为“0”则取“map[0]”，以此类推  

			rowNum = i / cols;   //求列编号  
			colNum = i % cols;   //―求行编号  
			x = colNum * tile;     //―求贴图X坐标   
			y = rowNum * tile;     //―求贴图Y坐标  

			BitBlt(mapdc, x, y, tile, tile, bufdc, 0, 0, SRCCOPY);  //在mdc上进行贴图  
		}
	}
	
	//object配置

	milk_listobj.clear();
	enemy_list.clear();
	npc_listnpc.clear();
	event_list.clear();
	gimmick_list.clear();

	switch (scene)
	{
	case 0:
		break;
	case 1:
	{
		manimation a_maid(6,6,"maid");
		npc maid1(0, a_maid);
		npc_listnpc.push_back(maid1);
		npc maid2(1, manimation(10,6,"maid"));
		npc_listnpc.push_back(maid2);
		npc maid3(2, manimation(14,6,"maid"));
		npc_listnpc.push_back(maid3);
		npc princess(3, manimation(16,6,"maid"));
		npc_listnpc.push_back(princess);

		event ev(3, 19*tile, 520);
		event_list.push_back(ev);
		break;
	}
		
	case 2:
	{
		manimation a_canon(10,8,"canon");
		enemy canon(1,1, 0, 0, a_canon);
		enemy_list.push_back(canon);
		//enemy bullet(2,1, 640, 500, -10, 0, "bullet", tile, tile,0);
		//enemy_list.push_back(bullet);

		manimation a_fire(7,6,"fire");
		enemy fire(100, 100, 0, 0, a_fire);
		enemy_list.push_back(fire);

		event back_title(0, tile*39, 520);
		event_list.push_back(back_title);
		break;
	}
		
	case 5:
	{
		manimation a_canon(7,6,"canon");
		enemy canon2(10,64,0,0,a_canon);
		enemy_list.push_back(canon2);
		boss_flag = true;
		event back_title(0, 19 * tile, 0);
		event_list.push_back(back_title);
		break;
	}
	case 3:
	{
		manimation a_heart(10,9,"hp_up");
		gimmick heart_max(255, a_heart);
		gimmick_list.push_back(heart_max);
		event back_title(0, 19*tile, 0);
		event_list.push_back(back_title);
		break;
	}
	case 4:
	{
		manimation a_rabbit(25,9,"platform");
		gimmick platform(1, a_rabbit);
		gimmick_list.push_back(platform);
		event back_title(0, 19 * tile, 0);
		event_list.push_back(back_title);
		break;
	}
	case 6:
	{
		manimation a_maid(5,6,"maid");
		npc maid1(4, a_maid);
		npc_listnpc.push_back(maid1);
		event ev3(1,6*tile,640);
		event_list.push_back(ev3);
		event ev4(2,tile*12,640);
		event_list.push_back(ev4);
		gimmick onsei(2, manimation(16,8,"onsei"));
		gimmick_list.push_back(onsei);
		enemy fire(2, 1, 0, 0, manimation(4,4,"fire"));
		enemy_list.push_back(fire);
		event back_title(0, 19*tile, 0);
		event_list.push_back(back_title);
		break;
	}
	}
	//plot setting
	switch (scene)
	{
	case 0:
		act = false;
		selector = true;
		break;
	case 1:
		act = true;
		selector = false;
		//false;
		//dialogue_next = "今天是公主殿下的生日。";
		break;
	case 2:
		act = true;
		break;
	case 3:
		act = true;
		break;
	case 4:
		act = true;
		break;
	case 5:
		
		act = true;
		break;
	}
	//roll setting
	switch (scene)
	{
	case 4:
		roll = 1;
		break;
	default:
		roll = 0;
		break;
	}
	if (scene > 0)
	{
		//milk reset
		milk.nHp = milk.mHp;
		milk.sprite.x = 0;
		milk.sprite.y = 0;
		//camera reset
		xview = 0;
	}	
}