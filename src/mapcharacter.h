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
#include "landmap.h"

#ifdef _EDIT_
#include "mapselect.h"
#include "mapobject.h"

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

#define STAND_NORTH 0
#define STAND_SOUTH 1
#define STAND_WEST 2
#define STAND_EAST 3
#define WALK_NORTH 4
#define WALK_SOUTH 5
#define WALK_WEST 6
#define WALK_EAST 7
#define NBR_MOVES 8

#define NO_MOVE 65535

class mapview;

class mapcharacter : public maptpl
{
 public:
  // Constructors and init functions
  void init();
  void clear();
  mapcharacter();
  ~mapcharacter();

  // Loading
  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  // Positioning
  void set_on_map(landmap * m) {refmap=m;}
  void remove_from_map() {refmap=NULL;}
  void set_pos(u_int16 smap,u_int16 x,u_int16 y);
  void set_offset(s_int8 x, s_int8 y) {offx=x; offy=y;}
  
  // Getting which movment the character is doing
  u_int16 move() {return current_move;}
  
  void update();
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void draw(mapview * mv);

  // Testing if a move is possible
  bool can_go_north();
  bool can_go_south();
  bool can_go_east();
  bool can_go_west();

  // Giving the character instructions on what it should do
  void stand_north();
  void stand_south();
  void stand_east();
  void stand_west();
  void go_north();
  void go_south();
  void go_east();
  void go_west();

  mapcharacter& operator =(mapcharacter &m);

 private:
  u_int16 current_move;
  u_int16 ask_move;
  u_int16 submap;
  u_int16 posx, posy;
  s_int8 offx, offy;
  vector<animation_off*> anim;
  landmap * refmap;
  
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
  void calculate_dimensions();


#ifdef _EDIT_
  // Editor specific functions
 public:
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
  friend class landmap;
};

#endif
