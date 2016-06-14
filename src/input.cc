/*
   $Id$

   Copyright (C) 1999/2000/2001  Alexandre Courbot. 
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   input.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the input class.
 * 
 * 
 */


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <SDL2/SDL.h>
#include "input.h"

u_int8 * input::keystate=NULL;
u_int8 * input::p_keystate=NULL;
bool input::text_input = false;
s_int32 input::keystatelength;

u_int16 input::mouse_posx, input::mouse_posy;
bool input::mouse_button[3];
std::queue<std::string> input::input_queue;

void input::init()
{
  const u_int8 *state = SDL_GetKeyboardState(&keystatelength);
  keystate=new u_int8[keystatelength];
  memcpy(keystate, state, keystatelength);

  //  set_keyboard_mode(MODE_STATE);
  p_keystate=new u_int8[keystatelength];
  memset(p_keystate, 0, keystatelength);
}

void input::shutdown()
{
  delete[] p_keystate;
}

void input::update()
{
	static SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
			{
				SDL_Scancode idx = event.key.keysym.scancode;
				if (!event.key.repeat)
				{
					keystate[idx]=1;
					p_keystate[idx]++;

					if (text_input)
					{
						if (idx == SDL_SCANCODE_BACKSPACE ||
							idx == SDL_SCANCODE_DELETE ||
							idx == SDL_SCANCODE_RETURN)
						{
							input_queue.push(std::string(1, (char)event.key.keysym.sym));
						}
					}
				}
				break;
			}
			case SDL_KEYUP:
			{
				SDL_Scancode idx = event.key.keysym.scancode;
				keystate[idx]=0;
				break;
			}
			case SDL_TEXTINPUT:
			{
				if (text_input)
				{
					input_queue.push(event.text.text);
				}
				break;
			}
		}
	}
}

bool input::is_pushed(SDL_Keycode key)
{
  bool ret;
  SDL_Scancode idx = SDL_GetScancodeFromKey(key);
  ret=keystate[idx];
  if((ret)&&(p_keystate[idx])) p_keystate[idx]--;
  return ret;
}

bool input::has_been_pushed(SDL_Keycode key)
{
  bool ret;
  SDL_Scancode idx = SDL_GetScancodeFromKey(key);
  ret=p_keystate[idx];
  if((ret)&&(!(--p_keystate[idx]))) keystate[idx]=0;
  return ret;
}

SDL_Keycode input::get_next_key()
{
  static SDL_Event event;
  static bool b;
  b=false;
  if(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_KEYDOWN,SDL_KEYDOWN)==1)
    {
      b=true;
      if(p_keystate[event.key.keysym.scancode]) p_keystate[event.key.keysym.scancode]--;
      keystate[event.key.keysym.scancode]=0;
    }
  if (b) return(event.key.keysym.sym);
  return(-1);
}

std::string input::get_next_unicode()
{
	if (!input_queue.empty())
	{
		std::string utf8_result(input_queue.front());
		input_queue.pop();
		return utf8_result;
	}
	return "";
}

void input::start_text_input()
{
	while(!input_queue.empty()) input_queue.pop();
	SDL_StartTextInput();
	text_input = true;
}

void input::stop_text_input()
{
	clear_keys_queue();
	text_input = false;
	SDL_StopTextInput();
}

void input::clear_keys_queue()
{
  memset(p_keystate, 0, keystatelength);
}
