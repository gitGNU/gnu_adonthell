/*
 * Cutscene classes: Part of the Adonthell project.
 *
 * This file is covered under the GNU GPL v2.0 or later. You know the drill.
 * 
 * (c)2000 Chris Harris (SAWDUST)
 *
 */
 
#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "types.h"
#include "input.h"
#include "gfx.h"
#include "animation.h"

typedef struct imagekey
{
  s_int16 x,y;
  bool visible; // 0 for no, 1 for yes
  u_int16 animspeed; // In game cycles per image
  imagekey *previmg;
  imagekey *nextimg;
  animation *anim;
} imagekey;


typedef struct keyframe
{
  s_int32 cycles; // How many game cycles till the next one
  keyframe *prevframe;
  keyframe *nextframe;
  imagekey *imagekeys;
} keyframe;

class cutscene : public image
{

 public:

  cutscene();
  ~cutscene();

  u_int8 insert_keyframe(u_int16 key); // Insert before specified key
  u_int8 delete_keyframe(u_int16 key); // Remove specific key

  u_int8 insert_imagekey(u_int8 ikey);
  u_int8 delete_imagekey(u_int8 ikey);
  u_int8 swap_imagekey(u_int8 ikeya, u_int8 ikeyb);

  u_int8 load(char * fname);
  u_int8 save(FILE *file);
  u_int8 render_scene();

  void set_cycles(u_int16 key, u_int16 cycles);
  void set_imagekey_anim(u_int8 img, animation * a);
  void set_imagekey_animspeed(u_int16 key, u_int8 img, u_int16 as);
  void set_imagekey_visible(u_int16 key, u_int8 img, bool visible);
  void set_coordinates(u_int16 key, u_int8 img, s_int16 x, s_int16 y);
  u_int16 get_current_keyframe();
  void set_current_keyframe(u_int16 key);
  void initialize_timer(void);

 private:

  u_int8 num_anims; // Number of images used in scene
  u_int16 num_keyframes; // Number of keyframes in scene
  u_int16 current_keyframe;
  s_int32 cyclecounter; // I wonder if it'll fill up...
  Uint32 timer;
  struct keyframe *current_keyframe_ptr;
  struct keyframe *keyframes; // Reference to first keyframe
  animation *anims; // Reference to first animation

};

#endif


