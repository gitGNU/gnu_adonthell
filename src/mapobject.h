/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef _MAPOBJ_H
#define _MAPOBJ_H

#include <vector>
#include "animation_off.h"
#include "maptpl.h"

class mapobject : public maptpl
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif

#ifdef _EDIT_
  win_font * font;
  win_theme * th;
  image * bg;
  win_container * container;
  win_label * label_status;
  win_label * label_part;

  char label_txt[500];
  u_int16 currentpart;
  bool must_upt_label_part;
  bool show_grid;
  bool in_editor;
#endif

  u_int16 length, height;
  animation_off * part;
  u_int16 * partref;
  u_int16 nbr_of_parts;

  void calculate_dimensions();

 public:
  void init();
  void clear();
  mapobject();
  ~mapobject();
  void play();
  void stop();
  void rewind();

  mapobject& operator =(const mapobject& mo);
  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
  u_int16 get_length();
  u_int16 get_height();
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif

  void update();
  void draw_free(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  void draw_border_free(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);
  void draw_border(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);
#ifdef _EDIT_
  void zoom(u_int16 sx, u_int16 sy, mapobject * src);
  void zoom_to_fit(u_int16 sm, mapobject * src);
  void init_parts();
  u_int16 increase_part(u_int16 c);
  u_int16 decrease_part(u_int16 c);
  void set_part_xoffset(u_int16 p, s_int16 xoff);
  void set_part_yoffset(u_int16 p, s_int16 xoff);
  void update_label_part();
  void set_currentpart(u_int16 p);
  void save();
  void load();
  void add_animation();
  s_int8 insert_animation(animation &an, u_int16 pos);
  s_int8 delete_animation(u_int16 pos);
  void set_part_offset(u_int16 partnbr, u_int16 x, u_int16 y);
  void resize_grid();
  void update_editor_keys();
  void update_editor();
  void draw_editor();
  void update_and_draw();
  void editor();
#endif
};

#endif
