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

  u_int16 posx;
  u_int16 posy;

  u_int16 d_posx;
  u_int16 d_posy;

 public:
  mapselect(u_int16 x, u_int16 y, u_int16 l=0, u_int16 h=0);
  ~mapselect();

  void check_if_fits();
  void move(u_int16 x, u_int16 y);
  void resize(u_int16 l, u_int16 h);

  void update();
  void draw();
};
