// $Id$

/*
    Copyright (C) 2000 Alexandre Courbot.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

    This file is a part of the Adonthell project.
*/

#ifndef _animation_h
#define _animation_h

#include "types.h"
#include "image.h"

#ifdef _EDIT_
#include <list>
#include <string>
#include "input.h"
#include "prefs.h"
#include "game.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_container.h"
#include "win_select.h"

#define IMAGE false
#define FRAME true

#endif

class animation_frame
{
  u_int16 imagenbr;
  bool is_masked;
  u_int8 alpha;
  s_int16 gapx;
  s_int16 gapy;
  u_int16 delay;
  u_int16 nextframe;
 public:
  void init();
  animation_frame();
  ~animation_frame();
  u_int8 get_alpha();
  void set_alpha(u_int8 a);
  bool get_mask();
  void set_mask(bool mask);
  u_int16 get_image();
  void set_image(u_int16 imnbr);
  u_int16 get_delay();
  void set_delay(u_int16 d);
  u_int16 get_nextframe();
  void set_nextframe(u_int16 nf);
  s_int8 get(SDL_RWops * file);
  s_int8 load(const char * fname);
#ifdef _EDIT_
  s_int8 put(SDL_RWops * file);
  s_int8 save(const char * fname);
#endif
  friend class animation;
#ifdef _EDIT_
  friend class animedit;
#endif
};

class animation
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif

#ifdef _EDIT_
  bool mode;
  static win_font * font;
  static win_border * border;
  static image * bg;
  win_container * container;
  win_label * label_mode;
  win_label * label_frame_nbr;
  win_label * label_frame_info;
  win_label * label_anim_info;

  u_int16 currentimage;
#endif

 protected:
  u_int16 nbr_of_images;
  u_int16 nbr_of_frames;
  u_int16 currentframe;
  u_int16 speedcounter;
  bool play_flag;

 public:
  image * t_frame;
  animation_frame * frame;

  animation();
  ~animation();

  void update();
  void set_active_frame(u_int16 framenbr);
  void next_frame();
  void play();
  void stop();
  void rewind();
  void draw(u_int16 x, u_int16 y);
  s_int8 get(SDL_RWops * file);
  s_int8 load(const char * fname);
#ifdef _EDIT_
  s_int8 put(SDL_RWops * file);
  s_int8 save(const char * fname);
#endif
  animation &operator =(animation &a);

#ifdef _EDIT_
  s_int8 insert_image(image &im, u_int16 pos);
  s_int8 insert_frame(animation_frame &af, u_int16 pos);
  u_int16 increase_frame(u_int16 c);
  u_int16 decrease_frame(u_int16 c);
  u_int16 increase_image(u_int16 c);
  u_int16 decrease_image(u_int16 c);
  void add_image();
  s_int8 delete_image(u_int16 pos);
  s_int8 delete_frame(u_int16 pos);
  void add_frame();
  void save();
  void load();
  void update_editor();
  void update_editor_keys();
  void draw_editor();
  void update_and_draw();
  void editor();
  void info_window(char * t_label);
  char * query_window(char * t_label);
#endif

};
#endif
