// $Id$
/*
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "screen.h"

#define MAPSQUARE_SIZE 20

class mapselect
{
  u_int16 s_posx;
  u_int16 s_posy;
  u_int16 length;
  u_int16 height;

  u_int16 d_posx;
  u_int16 d_posy;
  u_int16 dl;
  u_int16 dh;

  u_int16 posx;
  u_int16 posy;
  u_int16 cl;
  u_int16 ch;

 public:
  mapselect(u_int16 x, u_int16 y, u_int16 l, u_int16 h,
	    u_int16 d_l, u_int16 d_h);
  ~mapselect();

  void check_if_fits();
  void move(u_int16 x, u_int16 y);
  void resize(u_int16 l, u_int16 h);

  void set_cursor_pos(u_int16 x, u_int16 y);
  s_int8 move_cursor_right();
  s_int8 move_cursor_left();
  s_int8 move_cursor_up();
  s_int8 move_cursor_down();
  u_int16 get_cursor_x();
  u_int16 get_cursor_y();
  void update();
  void draw_cursor();
  void draw_grid();
  void draw();
};
