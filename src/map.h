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

#ifndef _map_h
#define _map_h

#include "types.h"
#include "mapsquare.h"
#include "mappattern.h"
#include "mapevent.h"
#include "window.h"

class mapitem;

typedef char lstr[100];

class map
{
  mapsquare ** themap;
  u_int16 maplong,maphaut;
  u_int16 mapx,mapy;
  s_int8 mapaddx,mapaddy;
  u_int8 scrolltype;
  u_int8 mapmovtype;
  u_int8 scridx;
  u_int8 mapspeeddelay,mapspeedcounter;
  u_int8 ATTACKABLE;
  mappattern * pattern;
  u_int16 nbr_of_patternsets;
  u_int16 nbr_of_patterns;
  mapcharacter heroe;
  int nbr_of_mapcharacters;       // u_int16!!!
  mapcharacter * othermapchar;
  u_int16 nbr_of_events;
  mapevent * event;
  mapitem * items;
  sprite toplayer;
  s_int16 * toplayerposx;
  s_int16 * toplayerposy;
  u_int16 toplayerflags;
  u_int16 toplayernbr;
  u_int16 toplayerspace;    /* Space between the tiles */
  u_int16 toplayerdecayh;    /* Decay between the tiles */
  u_int16 toplayerdecayv;    /* Decay between the tiles */
  s_int8 toplayerspeed;
  s_int8 toplayerspeedcounter;
  s_int8 toplayerparallaxspeed;
  s_int8 toplayerparallaxspeedcounter; 
  u_int16 toplayerhlimit;
  u_int16 toplayervlimit;
  u_int8 H_SCROLL_DISABLED;
  u_int8 V_SCROLL_DISABLED;
  lstr heroename;      // Change them to char * and modify
  lstr * patternname;    // getstringfromfile() so it allocate the string
  lstr * mapcharname;
  char toplayername[100];
  u_int8 status; /* Quitting? */
  
 public:
  window win;          /* status window */
  static const u_int8 depx=56;
  static const u_int8 depy=12;
  //  static u_int8 (*stdevent[256])(mapevent * ev,mapcharacter * aguy,map * amap,
  //				 u_int16 x, u_int16 y);

  map();
  // ~map();
  u_int16 get_patternset_to_map(FILE * file, u_int16 startpos);
  u_int16 load_patternset_to_map(char * fname, u_int16 startpos);
  s_int8 get(FILE * file);
  s_int8 load(char * fname);
  void init_for_scrolling();
  void follow(mapcharacter*aguy);
  void center_on(mapcharacter*aguy);
  void setspeeddelay(u_int8 sd);
  void update_status();
  void update_patterns();
  void update_pos();
  void update_all_characters();
  void update_keyboard();
  bool is_ready();
  u_int16 get_lenght();
  u_int16 get_height();
  u_int8 get_status();
  u_int8 get_scrolltype();
  void set_status(u_int8 st);
  void set_posx(u_int16 x);
  void set_posy(u_int16 y);
  void set_movtype(u_int16 mt);
  void set_scrolltype(u_int16 st);
  void enable_horizontal_scrolling();
  void disable_horizontal_scrolling();
  void enable_vertical_scrolling();
  void disable_vertical_scrolling();
  void put_character(u_int16 x, u_int16 y, mapcharacter * aguy);
  void leave_character(u_int16 x, u_int16 y);
  void drawdownsquare(int x, int y, mapsquare * msqr);
  void drawupsquare(int x, int y, mapsquare * msqr);
  void drawdownsquarepart(int x, int y, int w, int h, 
			  mapsquare * msqr, int xo, int yo);
  void drawupsquarepart(int x, int y, int w, int h, 
			mapsquare * msqr, int xo, int yo);
  void draw_down();
  void draw_up();
  void draw_character(mapcharacter * aguy);
  void draw_all_characters();
  bool is_unreachable(u_int16 x, u_int16 y);
  bool is_others_unreachable(u_int16 x, u_int16 y);
  u_int16 is_occuped(u_int16 x, u_int16 y);
  bool is_solid_up(u_int16 x, u_int16 y);
  bool is_solid_down(u_int16 x, u_int16 y);
  bool is_solid_left(u_int16 x, u_int16 y);
  bool is_solid_right(u_int16 x, u_int16 y);
  u_int16 get_square_eventcomenbr(u_int16 x, u_int16 y);
  u_int16 get_square_eventleavenbr(u_int16 x, u_int16 y);
  void launch_event(mapcharacter * aguy, u_int16 nbr);
  void run_event(u_int16 nbr, mapcharacter * aguy, u_int16 x, u_int16 y);
};

//#define TOP_LAYER_ACTIVE 128 /* Is the top layer drawn actually? */
//#define TOP_LAYER_ON 1        /* Is the top layer used? */
//#define TOP_LAYER_PARALLAX_MOVE 2   /* Parallax move? */
//#define TOP_LAYER_OFF 65534        /* Used to set the top layer off */
//#define TOP_LAYER_GO_RIGHT 4
//#define TOP_LAYER_GO_LEFT 8
//#define TOP_LAYER_GO_UP 16
//#define TOP_LAYER_GO_DOWN 32
//#define TOP_LAYER_DECAY 64          /* Use decay? */

#define MAP_STATUS_NORMAL 0
#define MAP_STATUS_QUIT 1
#define MAP_STATUS_FADE 2

#endif
