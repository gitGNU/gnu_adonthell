/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _mapcharacter_h
#define _mapcharacter_h

#include "image.h"
#include "character.h"

class landmap;
class mapcharacter
{
  // u_int16 posx;
  // u_int16 posy;
  s_int8 addx;
  s_int8 addy;
  u_int8 prevmovtype;
  u_int8 speeddelay;
  u_int8 speedcounter;
  u_int8 movtype;
  u_int16 framefactor;      /* u_int8 */
  u_int8 walkcounter;
  u_int8 moveframe;
  u_int8 scridx;
  u_int8 waitcounter;
  u_int16 nbr_of_frames;
  u_int8 is_event;
  u_int8 mapcharnbr;

 public:
  image * frame;
  u_int16 currentframe;
  character *data;

  mapcharacter();
  ~mapcharacter();
  s_int8 get(SDL_RWops * file);
  s_int8 load(char * fname);
  void get_heroe_stat(SDL_RWops * file);
  void put_heroe_stat(SDL_RWops * file);
  void get_NPC_stat(SDL_RWops * file, u_int16 nbr);
  void put_NPC_stat(SDL_RWops * file);
  void init_moveframe();
  u_int16 get_nbr();
  u_int16 get_posx();
  u_int16 get_posy();
  u_int8 get_addx();
  u_int8 get_addy();
  u_int8 get_scridx();
  u_int8 get_movtype();
  u_int8 get_speeddelay();
  void draw(u_int16 x, u_int16 y,drawing_area * da_opt=NULL);
  //  void draw_part(u_int16 x, u_int16 y, u_int16 bw, u_int16 bh, 
  //		 u_int16 xo, u_int16 yo);
  bool is_ready();
  bool is_speed_ready();
  void set_speeddelay(u_int8 sd);
  void set_movtype(u_int8 mt);
  void set_framefactor(u_int8 ff);
  void set_posx(u_int16 x);
  void set_posy(u_int16 y);
  void update_frame();
  void update_NPC(landmap * amap);
  void update_dir();
  void update_move(landmap *amap);
  void update_main_move(landmap *amap);
  void update_pos(landmap *amap);
  void update_eventleave(landmap *amap);
  void update_eventcome(landmap *amap);
};

#define MAPCHAR_EVENT_COME 1
#define MAPCHAR_EVENT_LEFT 2

#endif
