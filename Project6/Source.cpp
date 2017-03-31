//#include "stdafx.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment( lib, "dxguid.lib" )

#include<Windows.h>
#include <stdio.h>
#include<string.h>
#include<dinput.h>
#include<atlimage.h>
#include<list>
#include<vector>

#include"object.h"
#include"npc.h"
#include"enemy.h"
#include"player.h"

using namespace std;

#define DINPUT_BUFFERSIZE 16

#define BUTTON_UP DIK_W
#define BUTTON_DOWN DIK_S
#define BUTTON_LEFT DIK_A
#define BUTTON_RIGHT DIK_D
#define BUTTON_A DIK_L
#define BUTTON_B DIK_K
#define BUTTON_X DIK_O
#define BUTTON_Y DIK_J
#define BUTTON_L DIK_U
#define BUTTON_R DIK_I

const int tile_unit = 64;

//ȫ�ֱ�������

HINSTANCE hInst;
HBITMAP bg, sheep, girl, skill, skillult, slash, magic, recover, game,rabbit,fullmap;
HDC     hdc, mdc, bufdc,mapdc;
HWND    hWnd;
DWORD   tPre, tNow;
int     pNum, f, txtNum;
bool    attack, over,act,transform=false,trans_finish=false,boss_flag=false;
player milk(16,0,0,"rabbit",tile_unit,tile_unit);
char    text[5][100];
int num=0, num2=0,x1=0;
CString dialogue(""),dialogue_next("");

list<object> milk_listobj,event_listobj;
list<object>::iterator loi, loi2;

list<enemy> enemy_list;
list<enemy>::iterator lei;

list<npc> npc_listnpc;
list<npc>::iterator lni;


int rows, cols;

const int xscreen = 1280, yscreen = 720;


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



//ȫ�ֺ�������  
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                MyPaint(HDC hdc);


bool WallCheck(int x, int y);

bool check_in(int x, int y, object enemy)
{
	if (x >= enemy.x - enemy.sizeh / 2 + 1)
		if (x <= enemy.x + enemy.sizeh / 2)
			if (y >= enemy.y - enemy.sizev / 2 + 1)
				if (y <= enemy.y + enemy.sizev / 2)
					return true;
	return false;
}
bool damage_check(object milk, object enemy)
{
	//milk:def enemy:atk
	if (milk.invincible > 0)
		return false;
	//check four points
	if ((check_in(milk.x - 63, milk.y - 63, enemy)) || (check_in(milk.x + 64, milk.y - 63, enemy)) || (check_in(milk.x - 63, milk.y + 64,enemy)) || (check_in(milk.x + 64, milk.y + 64,enemy)))
		return true;
	return false;
}

void LoadMap(int room_index,HDC hdc,HDC bufdc,HDC mapdc);


//input
BOOL Device_Read(IDirectInputDevice8 *pDIDevice, DIDEVICEOBJECTDATA *didod,DWORD *dwElements,BYTE *diks)
{
	DWORD dwElements2;
	HRESULT hr;
	while (true)
	{
		pDIDevice->Poll();              // ��ѯ�豸  
		pDIDevice->Acquire();          // ��ȡ�豸�Ŀ���Ȩ  
		if (SUCCEEDED(hr = pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),didod,&dwElements2,0))&&SUCCEEDED(hr=pDIDevice->GetDeviceState(256,diks)))
		//if (SUCCEEDED(hr = pDIDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements2, 0)))
		//if (SUCCEEDED(hr = pDIDevice->GetDeviceState(256, diks))) 
			break;
		if (hr != DIERR_INPUTLOST || hr != DIERR_NOTACQUIRED) return FALSE;
		if (FAILED(pDIDevice->Acquire())) return FALSE;
	}
	*dwElements = dwElements2;
	//didod = didod2;
	/*for (int i = 0; i < dwElements2; i++)
	{
		
		if (didod2[i].dwOfs == DIK_J)
			if (didod2[i].dwData & 0x80)
			{
				object *new_bullet = new object(1, milk.x, milk.y, 10, 0, "bullet", 64);
				milk_listobj.push_back(*new_bullet);
			}
	}*/
	
	return TRUE;
}

//****WinMain������������ڵ㺯��**************************************  
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//��ʼ��    
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//��Ϣѭ��  
	GetMessage(&msg, NULL, NULL, NULL);            //��ʼ��msg      
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			tNow = GetTickCount();
			if (tNow - tPre >= 1000/60)
				MyPaint(hdc);
		}
	}
	g_pKeyboardDevice->Unacquire();
	g_pKeyboardDevice->Release();
	return msg.wParam;
}

//***���һ�������࣬��������⣬ʹ�ô��ڽṹ��*************************  
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

//****��ʼ������************************************  
//����λͼ���趨���ֳ�ʼֵ   
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//input
	//����DirectInput�豸  
	DirectInput8Create(hInstance, 0x0800, IID_IDirectInput8, (void**)&g_pDirectInput, NULL);
	g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, NULL);
	//�������ݸ�ʽ��Э������  
	g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//�趨��������С
	DIPROPDWORD property;
	property.diph.dwSize = sizeof(DIPROPDWORD);
	property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	property.diph.dwObj = 0;
	property.diph.dwHow = DIPH_DEVICE;
	property.dwData = DINPUT_BUFFERSIZE;
	g_pKeyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &property.diph);
	//��ȡ����Ȩ  
	g_pKeyboardDevice->Acquire();

	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Test", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 10, 10, xscreen, yscreen, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	mapdc = CreateCompatibleDC(hdc);
	

	LoadMap(1,hdc,bufdc,mapdc);
	bmp = CreateCompatibleBitmap(hdc, xscreen, yscreen);
	SelectObject(mdc, bmp);
	bg = (HBITMAP)LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, xscreen, yscreen, LR_LOADFROMFILE);
	
	
	//act = true;
	

	MyPaint(hdc);
	//ReleaseDC(hWnd, hdc);

	

	return TRUE;
}

//****�Զ����ͼ����*********************************  
// 1.������ͼ���ս��Ϣ��ʾ  
// 2.������Ϊ�жϼ��������ݴ��������  
void MyPaint(HDC hdc)
{

	//���ϱ���ͼ  
	SelectObject(bufdc, bg);
	BitBlt(mdc, 0, 0, xscreen, yscreen, bufdc, 0, 0, SRCCOPY);
	
	

	char str[100];
	int i, damage;

		

	
	//-----------------------------------------INPUT PROCESS------------------------------
	//input read
	Device_Read(g_pKeyboardDevice, didod, &dwElements,g_pKeyStateBuffer);
	//press control
	for (DWORD i = 0; i < dwElements; i++)
	{
		if (act)
		{
			//act mode
			{
				if (didod[i].dwData&0x80)
					switch (didod[i].dwOfs)
					{
					case BUTTON_X:
						if (milk.costume != 0)
							milk.costume = 0;
						break;
					case BUTTON_Y:
						if (!milk.costume)
						{
							if (!transform)
							{
								object *new_bullet = new object(1, milk.x, milk.y, 10, 0, "bullet", 64, tile_unit);
								milk_listobj.push_back(*new_bullet);
							}
						}
						break;
					case BUTTON_B:
						if (!transform)
						{
							if (WallCheck(milk.x - tile_unit / 2, milk.y + tile_unit) || WallCheck(milk.x + tile_unit / 2, milk.y + tile_unit))
							{
								milk.vy = -20;
							}
						}
						break;
					case BUTTON_UP:
						if (!transform)
						{
							//npc check
							for (lni = npc_listnpc.begin(); lni != npc_listnpc.end(); ++lni)
							{
								if (damage_check(milk, *lni))
								{
									act = false;
									dialogue_next = lni->txt;
									break;
								}
							}
						}
						else
						{
							milk.costume = 1;
							trans_finish = true;
						}
						break;
					case BUTTON_DOWN:
						if (transform)
						{
							//milk.costume = 2;
							//trans_finish = true;
						}
							
						break;
					case BUTTON_LEFT:
						if (transform)
						{
							//milk.costume = 3;
							//trans_finish = true;
						}
							
						break;
					case BUTTON_RIGHT:
						if (transform)
						{
							//milk.costume = 4;
							//trans_finish = true;
						}
							
						break;
					}	
			}
			
			
		}
		else
		{
			if (didod[i].dwOfs == BUTTON_A)
				if (didod[i].dwData & 0x80)
				{
				int temp = dialogue_next.Find('\t');
				if (temp != -1)
				{
					dialogue = dialogue_next.Left(temp);
					dialogue_next = dialogue_next.Mid(temp + 1);
				}
				else
				{
					dialogue = dialogue_next;
					dialogue_next = "";
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
			if (!trans_finish)
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
			if (g_pKeyStateBuffer[DIK_S] & 0x80)
			{

			}
			if (g_pKeyStateBuffer[BUTTON_LEFT] & 0x80)
			{
				while (WallCheck(milk.x - 10 - tile_unit / 2, milk.y) || WallCheck(milk.x - 10 - tile_unit / 2, milk.y + tile_unit - 1))
					milk.x++;
				milk.x -= 10;
			}
			if (g_pKeyStateBuffer[BUTTON_RIGHT] & 0x80)
			{
				while (WallCheck(milk.x + 10 + tile_unit / 2, milk.y) || WallCheck(milk.x + 10 + tile_unit / 2, milk.y + tile_unit - 1))
					milk.x--;

				milk.x += 10;
			}
		}
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
			milk.y += milk.vy;

			//������
			if (WallCheck(milk.x - tile_unit / 2, milk.y + tile_unit) || WallCheck(milk.x + tile_unit / 2, milk.y + tile_unit))
			{
				milk.y = milk.y / tile_unit * tile_unit;
				milk.vy = 0;
			}

			else
				milk.vy += 1;
			//bounder check
			while (milk.x < 0)
				milk.x++;
			//milk bullet phase
			for (loi = milk_listobj.begin(); loi != milk_listobj.end();)
			{
				//milk bullet behavior
				loi->motion();
				if (loi->x<xview || loi->x>xview + xscreen)
				{
					loi = milk_listobj.erase(loi);
					continue;
				}

				//enemy_damage_check
				for (lei = enemy_list.begin(); lei != enemy_list.end();)
				{
					if (damage_check(*lei, *loi))
					{
						lei->nHp -= 1;
						if (lei->nHp == 0)
							lei = enemy_list.erase(lei);
						else
							lei++;
					}
					else
						lei++;
				}

				loi++;

			}
			
			//event check
			for (loi = event_listobj.begin(); loi != event_listobj.end(); )
			{
				if (damage_check(milk, *loi))
				{
					loi = event_listobj.erase(loi);
					if (milk.x > 1280)
						LoadMap(3, hdc, bufdc, mapdc);
					else
						LoadMap(2, hdc, bufdc, mapdc);
					break;
				}
				else
					loi++;
			}


			//-----------------------------------------------ENEMY PHASE
			for (lei = enemy_list.begin(); lei != enemy_list.end(); )
			{
				//enemy behavior
				lei->thinking();
				lei->UseSkill();
				lei->update();
				lei->motion();
				if (lei->x<xview || lei->x>xview + xscreen)
				{
					lei = enemy_list.erase(lei);
					continue;
				}

				//milk_damage_check
				if (damage_check(milk, (object)*lei))
				{
					milk.nHp -= 1;
					milk.invincible = 60;
				}
				lei++;
			}
		}
		
			
				
				
	}
	else
	{
		if (dialogue!="")
		{
			TextOut(mdc, 100, 0, dialogue, strlen(dialogue));
			
		}
		else if (dialogue_next!="")
		{
			int temp = dialogue_next.Find('\t');
			if (temp != -1)
			{
				dialogue = dialogue_next.Left(temp);
				dialogue_next = dialogue_next.Mid(temp + 1);
			}
			else
			{
				dialogue = dialogue_next;
				dialogue_next = "";
			}
		}
		else
			act = true;
		
	}
	
	//---------------------------------------------------DRAW
	//camera
	if (milk.x - xview > xscreen / 2 && cols * tile_unit - milk.x > xscreen / 2)
		xview = milk.x - xscreen / 2;
	if (milk.x - xview < xscreen / 2 && milk.x > xscreen / 2)
		xview = milk.x - xscreen / 2;

	CImage image;
	//npc draw
	for (lni = npc_listnpc.begin(); lni != npc_listnpc.end(); ++lni)
	{
		lni->draw(mdc);

		if (damage_check(milk, *lni))
		{
			if (act)
			{
				image.Load("E:\\up.png");
				image.Draw(mdc, milk.x - tile_unit*1.5, milk.y - tile_unit, tile_unit, tile_unit, 0, 0, tile_unit, tile_unit);
				image.Destroy();
			}
		}

	}
	//enemy draw
	for (lei = enemy_list.begin(); lei != enemy_list.end(); ++lei)
	{
		lei->draw(mdc);
	}
	//milk draw
	
	image.Load("E:\\rabbit.png");

	if (!(milk.invincible % 2))
		image.Draw(mdc, milk.x - xview - tile_unit / 2, milk.y, tile_unit, tile_unit, x1, 0, tile_unit, tile_unit);
	image.Destroy();
	num2++;
	if (num2 == 10)
	{
		num2 = 0;
		num++;
		if (num == 4)
			num = 0;
	}
	x1 = tile_unit * num;
	//milk bullet
	for (loi = milk_listobj.begin(); loi != milk_listobj.end(); ++loi)
	{
		loi->draw(mdc);
	}
	
		
	//UI
	//CImage image;
	image.Load("E:\\face.png");
	image.Draw(mdc, 0, 0, tile_unit*2,tile_unit*2,0,0,tile_unit*2,tile_unit*2);
	image.Destroy();
	image.Load("E:\\heart.png");
	//int i;
	for (i = 0; i < milk.nHp/4;i++)
		image.Draw(mdc, (i+2)*tile_unit, 0, tile_unit, tile_unit, tile_unit*4, 0, tile_unit, tile_unit);
	if (i<milk.mHp/4)
		image.Draw(mdc, (i + 2)*tile_unit, 0, tile_unit, tile_unit, tile_unit*(milk.nHp % 4), 0, tile_unit, tile_unit);
	i++;
	for (; i < milk.mHp/4; i++)
		image.Draw(mdc, (i+2)*tile_unit, 0, tile_unit, tile_unit, 0, 0, tile_unit, tile_unit);
	image.Destroy();
	

	sprintf_s(str, "%d / %d", milk.nHp, milk.mHp);
	TextOut(mdc, 0, 0, str, strlen(str));
	//boss hp
	if (boss_flag)
	{
		image.Load("E:\\face.png");
		image.Draw(mdc, 1100, 500, tile_unit * 2, tile_unit * 2, 0, 0, tile_unit * 2, tile_unit * 2);
		image.Destroy();
		sprintf_s(str, "%d / %d", enemy_list.begin()->nHp, enemy_list.begin()->mHp);
		TextOut(mdc, 1100, 400, str, strlen(str));
	}
	//map
	//SelectObject(mapdc, fullmap);
	BitBlt(mdc, 0, 0, xscreen, yscreen, mapdc, xview, 0, SRCAND);
	//display
	//SelectObject(mdc, bmp);
	BitBlt(hdc, 0, 0, xscreen, yscreen, mdc, 0, 0, SRCCOPY);
	

	tPre = GetTickCount();

	pNum++;
	if (pNum == 8)
		pNum = 0;
}



bool WallCheck(int x, int y)
{
	if (mapIndex[y / tile_unit * cols + x / tile_unit] != 0)
		return 1;
	else
		return 0;
}

//****��Ϣ������***********************************  
//   
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	switch (message)
	{
	case WM_KEYDOWN:                //������Ϣ  
		if (wParam == VK_ESCAPE)       //����Esc��  
			PostQuitMessage(0);
		
		break;
	case WM_LBUTTONDOWN:            //��������Ϣ  
		if (!attack)
		{
			x = LOWORD(lParam);     //X����  
			y = HIWORD(lParam);     //Y����  

			if (x >= 500 && x <= 550 && y >= 350 && y <= 400)
				attack = true;
		}
		break;
	case WM_DESTROY:                //���ڽ�����Ϣ  
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteDC(mapdc);
		DeleteObject(bg);
		DeleteObject(sheep);
		DeleteObject(girl);
		DeleteObject(skill);
		DeleteObject(skillult);
		DeleteObject(slash);
		DeleteObject(magic);
		DeleteObject(recover);
		DeleteObject(game);
		DeleteObject(fullmap);

		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);
		break;
	default:                        //Ĭ����Ϣ  
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//load map
void LoadMap(int room_index,HDC hdc,HDC bufdc,HDC mapdc)
{
	//map
	switch (room_index)
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
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		break;
	case 2:
		rows = 12;
		cols = 40;
		mapIndex = { 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		break;
	case 3:
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
	}
		

	

	fullmap = CreateCompatibleBitmap(hdc, cols * tile_unit, rows * tile_unit); //�Ƚ���fullmapΪ�հ׵�λͼ���������߷ֱ�Ϊ������*ͼ����롰����*ͼ��ߡ���  
	SelectObject(mapdc, fullmap);

	HBITMAP map[3];
	char filename[20] = "";
	int rowNum, colNum;
	int i, x, y;

	//���ظ���λͼ  
	for (i = 0; i<3; i++)  //����ѭ��ת��ͼ���ļ�����ȡ������ͼ����롰map[i]���С�ͼ���ļ���Ϊ��map0.bmp���͡�map1.bmp���ȡ�  
	{
		sprintf_s(filename, "map%d.bmp", i);
		map[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, tile_unit, tile_unit, LR_LOADFROMFILE);
	}

	//����mapIndex�����еĶ���ȡ����Ӧͼ�飬���е�ͼƴ��   
	for (i = 0; i<rows*cols; i++)
	{
		SelectObject(bufdc, map[mapIndex[i]]);  //����mapIndex[i]�еĴ���ѡȡ��Ӧ��ͼ�鵽bufdc�С�����Ϊ��0����ȡ��map[0]�����Դ�����  

		rowNum = i / cols;   //���б��  
		colNum = i % cols;   //�D���б��  
		x = colNum * tile_unit;     //�D����ͼX����   
		y = rowNum * tile_unit;     //�D����ͼY����  

		BitBlt(mapdc, x, y, tile_unit, tile_unit, bufdc, 0, 0, SRCCOPY);  //��mdc�Ͻ�����ͼ  
	}

	//object����
	milk_listobj.clear();
	enemy_list.clear();
	npc_listnpc.clear();
	event_listobj.clear();

	switch (room_index)
	{
	case 1:
	{
		npc maid1(500, 64*8, "maid", tile_unit,tile_unit*2,"�����ǹ������µ����գ����ǹ������½��꼸���أ�");
		npc_listnpc.push_back(maid1);
		npc maid2(600, 64*8, "maid", tile_unit,tile_unit*2, "���ǻʼ��ö����Ǿ����ϸ���ѡ��ѵ�����������ľ�Ӣ�ö���\t��ʹ����Ҳ��������ȫһ���ɡ�");
		npc_listnpc.push_back(maid2);
		npc maid3(700, 64 * 8, "maid", tile_unit, tile_unit * 2, "��������е�����أ�\t��ô��\t��Ķ���̣ܶ���Ե�β��ȴ�ܳ���\t��������ңԶ�Ŀ��������ľ����������Ӻͱ����ľ�����е���������\tԭ��������˰������ǿ���������û��˵���ء�");
		npc_listnpc.push_back(maid3);
		npc princess(1200, 64 * 8, "maid", tile_unit, tile_unit * 2, "2");
		npc_listnpc.push_back(princess);

		object ev(1, 1200, 520, 0, 0, "rabbit", tile_unit,tile_unit);
		event_listobj.push_back(ev);
		break;
	}
		
	case 2:
	{
		enemy canon(1,1, 640, 500, 0, 0, "canon", tile_unit * 2, tile_unit * 2);
		enemy_list.push_back(canon);
		enemy bullet(2,1, 640, 500, -10, 0, "bullet", tile_unit, tile_unit);
		enemy_list.push_back(bullet);

		object ev2(1, 2400, 520, 0, 0, "rabbit", tile_unit, tile_unit);
		event_listobj.push_back(ev2);
		break;
	}
		
	case 3:
	{
		enemy canon2(10,64, 640, 500,0,0, "canon", tile_unit * 2, tile_unit * 2);
		enemy_list.push_back(canon2);
		boss_flag = true;
		break;
	}
		
	}
	//plot setting
	switch (room_index)
	{
	case 1:
		act = false;
		dialogue_next = "�����ǹ������µ����ա�";
		break;
	case 3:
		act = false;
		dialogue_next = "boss";
		break;
	}
	//milk position reset
	milk.x = 0;
	milk.y = 0;
	//camera reset
	xview = 0;
	

	
}