/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef _MAPCHARACTER_H
#define _MAPCHARACTER_H

#include <vector>
#include "animation_off.h"
#include "maptpl.h"
#ifdef _EDIT_
#include "mapselect.h"

#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_anim.h"
#include "win_container.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_background.h"
#endif

class mapcharacter : public maptpl
{
#define STAND_NORTH 0
#define STAND_SOUTH 1
#define STAND_WEST 2
#define STAND_EAST 3
#define WALK_NORTH 4
#define WALK_SOUTH 5
#define WALK_WEST 6
#define WALK_EAST 7
#define NBR_MOVES 8

  u_int16 current_move;

  vector<animation_off*> anim;
  
  u_int16 length, height;

#ifdef _EDIT_
  char label_txt[500];
  image * bg;
  win_font * font;
  win_theme * th;

  win_container * container;
  win_label * label_frame;
  win_label * label_char;

  bool must_upt_label_frame;
  bool must_upt_label_char;

  bool show_grid;
#endif

 public:
  void init();
  void clear();
  mapcharacter();
  ~mapcharacter();

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  void update();
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);

  void go_north();
  void go_south();
  void go_east();
  void go_west();

  mapcharacter& operator =(mapcharacter &m);
  void calculate_dimensions();
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);

  void load();
  void save();

  void insert_anim(animation_off * an, u_int16 pos);
  void load_anim();
  void update_editor();
  void set_anim_xoffset(u_int16 p, s_int16 xoff);
  void set_anim_yoffset(u_int16 p, s_int16 yoff);
  void update_label_frame();
  void update_label_char();
  void draw_editor();
  void update_and_draw();
  void update_editor_keys();
  void editor();
#endif
};

#endif
