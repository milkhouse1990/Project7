#define FPS 60
#define BYTEPERLINE 30
#include <fstream>
#include <string>
#include<vector>
#include<sstream>
#include "avg_engine.h"
using namespace std;
void avgengine::boot(CString name)
{
	CString title = "Line\\";
	title = title + name + ".txt";
	string s_title(title.GetBuffer(title.GetLength()));

	file.open(s_title.c_str(), ios::_Nocreate);
	if (!file.is_open())
	{
		errmsg = "can't open file: " + title;
		err = true;
	}
	/*
	avgend = false;

	speaker = "";
	words = "";
	linelength = 20;
	chaid2obj[1] = obj_character;
	chaid2obj[2] = obj_npc_chaya;
	chaid2obj[3] = obj_npc_hime;
	chaid2obj[4] = obj_npc_lei;

	para[0] = "";
	*/


}

bool avgengine::display(HDC mdc)
{
	if (dialogue != "")
	{
		TextOut(mdc, 100, 0, dialogue, strlen(dialogue));
		return false;
	}
	else if (dialogue_next != "")
	{
		nextpage();
		return false;
	}
	else
		return true;
}
void avgengine::nextpage()
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


bool avgengine::step()
{
	string str1;
	vector<CString> para;
	stringstream ss;
	int int_temp;
	/*if keyboard_check_pressed(global.my_START)
	{
	file_text_close(file);
	file = -1;
	avgend = true;
	}

	if pause
	{
	if keyboard_check_pressed(global.my_A)
	{
	file_text_readln(file);
	pause = false;
	err = false;
	}
	}
	else if wait
	{
	}
	else*/
	if (!pause && !wait)
	{
		//command load
		if (getline(file, str1))
		{
			//command analysis
			//int i = 0;
			CString cstr(str1.c_str());
			while (cstr != "")
			{
				int index = cstr.Find(' ');
				if (index != -1)
				{
					para.push_back(cstr.Left(index));
					cstr = cstr.Mid(index + 1);
				}
				else
				{
					para.push_back(cstr);
					cstr = "";
				}
			}
			//command understanding
			//pause time
			if (para[0] == "pause")
			{
				wait = true;
				ss << para[1];
				ss >> int_temp;
				ss.clear();
				alarm = int_temp*FPS;
			}
			//say charaid text
			else if (para[0] == "say")
			{
				pause = true;
				speaker = para[1];
				words = para[2];
				for (size_t j = 3; j < para.size(); j++)
				{
					words += " ";
					words += para[j];
				}


				words0.clear();
				while (words.GetLength()>BYTEPERLINE)
				{
					words0.push_back(words.Left(BYTEPERLINE));
					words = words.Mid(BYTEPERLINE);

				}
				words0.push_back(words);
				words = "";

				al = true;
				alarm = 10;
			}
			//charaset charaid x y
			else if (para[0] == "charaset")
			{

				//charaid = real(para[1]);
				//charax = real(para[2]);
				//charay = real(para[3]);
				//time=para[4];
				//instance_create(charax, charay, chaid2obj[charaid]);

			}
			//charaunset charaid
			else if (para[0] == "charaunset")
			{
				//wait=true;
				//time=para[2];
				//with(chaid2obj[real(para[1])])instance_destroy();
				//alarm[0]=time*room_speed;
				//file_text_readln(file);
			}
			//charascale charaid xscale yscale
			else if (para[0] == "charascale")
			{
				//charaid = real(para[1]);
				//charax = real(para[2]);
				//charay = real(para[3]);
				//chaid2obj[charaid].image_xscale = charax;
				//chaid2obj[charaid].image_yscale = charay;

				//file_text_readln(file);
			}
			//charaanime charaid index
			else if (para[0] == "charaanime")
			{
				//chaid = real(para[1]);
				//chaid2obj[chaid].image_index=chaidind2spr[chaid,real(para[2])];
				//file_text_readln(file);
			}
			//charamove charaid vx vy
			else if (para[0] == "charamove")
			{
				//chaid = real(para[1]);
				//if instance_exists(chaid2obj[chaid])
				//{
				//	chaid2obj[chaid].hspeed = real(para[2]);
				//	chaid2obj[chaid].vspeed = real(para[3]);
				//	file_text_readln(file);
				//}
				//else
				//{
				//	err = true;
				//	errmsg = "can't find object " + para[1];
				//	pause = true;
				//}
			}
			//error
			else
			{
				errmsg = "can't understand command: " + para[0];
				err = true;
				wait = true;
			}
			return false;
		}
		else
		{
			file.close();
			return true;
			//avgend = true;
		}
	}
	return false;


}
void avgengine::draw(HDC mdc, int xview, int yview)
{
	
	int pos;
	if (err)
	{
		TextOutA(mdc, 100, 0, errmsg.c_str(), strlen(errmsg.c_str()));
	}
	else if (pause)
	{
		//UI
		pos = 30;
		
		/*
		//milk
		if (speaker = "1")
		{
		//cos
		draw_sprite(spr_avatar_milk, global.necklace, view_xview + 5, view_yview + 5)
		//face
		if health > 8
		{
		draw_sprite(spr_avatar_milkf, 0, view_xview + 5, view_yview + 5);
		}
		else
		{
		draw_sprite(spr_avatar_milkf, 2, view_xview + 5, view_yview + 5);
		}
		image_speed = 0;
		}
		//chara

		else
		{
		draw_sprite(spr_avatar, real(speaker) - 2, view_xview + 635, view_yview + 5)
		}

		//dialogue
		draw_sprite(spr_dialogue, 0, view_xview + pos, view_yview + 5)
		draw_sprite_ext(spr_nextpage, 0, view_xview + 450 + pos, view_yview + 70, 1, 1, 1, c_white, al)

		*/


		//TALK
		//draw_text(0, 0, speaker);
		//int i = 0;

		/*if (words != "")
		{

		words = "";
		}*/
		if (speaker == "1")
			icon.draw(mdc);
		frame.draw(mdc);
		np.draw(mdc);
		for (size_t i = 0; i < words0.size(); i++)
		{
			char *cwords0 = NULL;
			//strcpy_s(cwords0, sizeof(words0[i]),words0[i]);
			TextOut(mdc, xview + 15 + pos, yview + 15 + i * 16, words0[i].GetBuffer(0), words0[i].GetLength());
		}
	}

	//else if para[0] == "charaset"
	{
	}




}


		
		