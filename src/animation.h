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

#ifdef _EDIT_
typedef enum anim_editor_mode {IMAGE, FRAME};
#endif

#endif

class animation_frame
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif
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
  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif
  friend class animation;
#ifdef _EDIT_
  friend class animedit;
#endif

  friend class win_anim;
  friend class animation_off;
};

class animation
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#endif

#ifdef _EDIT_
  image * clipboard;
  animation_frame f_clipboard;
  bool mode;
  char frame_txt[500];
  win_font * font;
  win_border * border;
  image * bg;
  win_container * container;
  win_label * label_mode;
  win_label * label_frame_nbr;
  win_label * label_frame_info;
  win_label * label_anim_info;
  bool must_upt_label_mode;
  bool must_upt_label_frame_nbr;
  bool must_upt_label_frame_info;
  bool must_upt_label_status;

  u_int16 currentimage;
  bool in_editor;
#endif

 protected:
  u_int16 length, height;
  u_int16 nbr_of_images;
  u_int16 nbr_of_frames;
  u_int16 currentframe;
  u_int16 speedcounter;
  bool play_flag;

 public:
  image * t_frame;
  animation_frame * frame;

  void init();
  animation();
  void clear();
  ~animation();
  bool is_empty();
  u_int16 get_length();
  u_int16 get_height();

  void update();
  void set_active_frame(u_int16 framenbr);
  void next_frame();
  void play();
  void stop();
  void rewind();
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);
  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  void zoom(u_int16 sx, u_int16 sy, animation * src);
  void get_zoom_scale(u_int16 &max_x, u_int16 &max_y);

#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif
  animation &operator =(animation &a);

#ifdef _EDIT_
  void select_image(u_int16 nbr);
  void select_frame(u_int16 nbr);
  void set_frame_gapx(u_int16 nbr, s_int16 gx);
  void set_frame_gapy(u_int16 nbr, s_int16 gy);
  void set_frame_alpha(u_int16 nbr, u_int8 a);
  void set_frame_delay(u_int16 nbr, u_int16 d);
  void set_frame_nextframe(u_int16 nbr, u_int16 nf);
  void set_frame_mask(u_int16 nbr, bool m);
  void set_frame_imagenbr(u_int16 nbr, u_int16 imnbr);
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
  void set_mode(anim_editor_mode m);
  void update_label_mode();
  void update_label_frame_nbr();
  void update_label_frame_info();
  void update_label_status();
  void update_editor();
  void update_editor_keys();
  void update_and_draw();
  void editor();
  void info_window(char * t_label);
  char * query_window(char * t_label);
#endif

  friend class win_anim;
  friend class animation_off;
};

#endif
