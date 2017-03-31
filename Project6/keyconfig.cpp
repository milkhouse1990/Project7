#include <fstream>
#include <string>
#include <atlimage.h>
#include <sstream>
#include <io.h>
#include <direct.h>
#include "keyconfig.h"
using namespace std;
bool keyconfig::load_config(int *up,int *down,int *left,int *right,int *a,int *b,int *x,int *y,int *l,int *r,int *select,int *start)
{
	ifstream file("Config\\keyconfig.txt", ios::_Nocreate);
	
	if (!file.is_open())
		return false;
	//check content
	int i;
		for (i = 0; i < 12; i++)
		{
			string s_keys;
			getline(file, s_keys);
			CString cs_keys(s_keys.c_str());
			int index=cs_keys.Find('=');
			stringstream ss;
			//ss.clear();
			ss << cs_keys.Mid(index + 1);
			int keys;
			ss >> keys;
			if (keys <= 1 || keys>0xED)
				break;
			if (i==0)
			{
				if (cs_keys.Left(index) == "up")
					*up = keys;
				else
					break;
			}
			else if (i==1)
			{
				if (cs_keys.Left(index) == "down")
					*down=keys;
				else
					break;
			}
			else if (i == 2)
			{
				if (cs_keys.Left(index) == "left")
					*left = keys;
				else
					break;
			}
			else if (i==3)
			{
				if (cs_keys.Left(index) == "right")
					*right=keys;
				else
					break;
			}
			else if (i==4)
			{
				if (cs_keys.Left(index) == "a")
					*a=keys;
				else
					break;
			}
			else if (i==5)
			{
				if (cs_keys.Left(index) == "b")
					*b=keys;
				else
					break;
			}
			else if (i==6)
			{
				if (cs_keys.Left(index) == "x")
					*x=keys;
				else
					break;
			}
			else if (i==7)
			{
				if (cs_keys.Left(index) == "y")
					*y=keys;
				else
					break;
			}
			else if (i==8)
			{
				if (cs_keys.Left(index) == "l")
					*l=keys;
				else
					break;
			}
			else if (i==9)
			{
				if (cs_keys.Left(index) == "r")
					*r=keys;
				else
					break;
			}
			else if (i==10)
			{
				if (cs_keys.Left(index) == "select")
					*select=keys;
				else
					break;
			}
			else if (i==11)
			{
				if (cs_keys.Left(index) == "start")
					*start=keys;
				else
					break;
			}
		}
		file.close();
		if (i == 12)
			return true;
		else
			return false;
}
void keyconfig::restore_config()
{
	if (_access("Config\\keyconfig.txt", 6) == -1)
		_mkdir("Config\\");
	ofstream file("Config\\keyconfig.txt", ios::ate);
	if (file.is_open())
	{
		file << "up=17\n";
		file << "down=31\n";
		file << "left=30\n";
		file << "right=32\n";
		file << "a=38\n";
		file << "b=37\n";
		file << "x=24\n";
		file << "y=36\n";
		file << "l=22\n";
		file << "r=23\n";
		file << "select=2\n";
		file << "start=3\n";
		file.close();
	}
	
}