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

// An animation frame is an extension to the image class. It contains
// a delay factor and a gap parameter so images of different sizes can be
// used in the same animation.

class animation_frame : public image
{
  s_int16 gapx;
  s_int16 gapy;
  // If delay is 0, the delay factor is not taken in account
  // Otherwise the animation will wait "delay" game cycles to switch to
  // the other frame
  u_int16 delay;
  bool next_nostd;
  u_int16 nextframe;
 public:
  void init();
  animation_frame();
  ~animation_frame();
  s_int8 get(SDL_RWops * file);
  s_int8 load(const char * fname);
#ifdef _EDIT
  s_int8 put(SDL_RWops * file);
  s_int8 save(const char * fname);
#endif
  friend class animation;
#ifdef _EDIT
  friend class animedit;
#endif
};

// An animation is mainly a dynamic array of animation_frames. 
// The animation can be controlled by 2 ways: by setting a delay to a frame
// or (in case animation_frame::delay==0) by calling next_frame();
// alpha_on, alpha and mask_on can be modified to control the animation
// parameters.

class animation
{
#ifdef DEBUG
  static u_int16 a_d_diff;
#endif
 protected:
  u_int16 nbr_of_frames;
  u_int16 currentframe;
  u_int16 speedcounter;
  s_int8 factor;
  bool play_flag;
  //  void init_frame(u_int16 nbr);

 public:
  animation_frame * frame;
  bool loop;              // Shall the animation loop?
  bool reverse;  // Shall the animation reverse when reaching the last frame?

  animation();
  ~animation();

  // Check if it's time to switch to the next frame
  void update();
  void draw(u_int16 x, u_int16 y);
  void set_active_frame(u_int16 framenbr);
  void increase_frame();
  void decrease_frame();
  void next_frame();
  void play();
  void stop();
  void rewind();
  // Increase nbr_of_frames and load the frame to the last position
  s_int8 load_frame(const char * fname, u_int16 pos);
  s_int8 get_frame(SDL_RWops * file, u_int16 pos);
  s_int8 delete_frame(u_int16 pos);
  s_int8 get(SDL_RWops * file);
  s_int8 load(const char * fname);
#ifdef _EDIT
  s_int8 put(SDL_RWops * file);
  s_int8 save(const char * fname);
#endif
  void set_delay(u_int16 framenbr, u_int16 delay);
  void set_next_frame(u_int16 framenbr, u_int16 next_frame);
  void set_mask(u_int16 framenbr, bool m);
  void set_alpha(u_int16 framenbr, bool a, u_int8 value);
};
#endif
