// $Id
/*
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "keyboard.h"
#include "gfx.h"
#include "globals.h"
#ifdef SDL
#include "SDL.h"
#include "SDL_thread.h"
#include "keyboard.h"
#endif
#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#endif
#include "animation.h"
#include "cutscene.h"

int main(int argc, char * argv[])
{
  cutscene scene;
  animation anim;
  animation anim2;
  u_int8 ret=0;

  screen::init_display(0);

#ifdef SDL
  SDL_Thread *input_thread;

   input_thread = SDL_CreateThread((void*)keyboard_init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }
#endif

  anim.load_frame("test1.pnm");
  anim.load_frame("test2.pnm");
  anim.load_frame("test3.pnm");
  anim.loop=false;
  anim.reverse=true;
  anim.mask_on=true;
  anim.alpha_on=false;
  anim.alpha=0;

  anim2.load_frame("bg1.pnm");
  anim2.load_frame("bg2.pnm");
  anim2.load_frame("bg3.pnm");
  anim2.loop=false;
  anim2.reverse=true;
  anim2.mask_on=true;
  anim2.alpha_on=false;
  anim2.alpha=0;

  scene.insert_keyframe(1);
  scene.insert_imagekey(0);

  scene.set_imagekey_anim(0,&anim2);
  scene.set_imagekey_anim(1,&anim);

  scene.set_imagekey_animspeed(0,1,10);
  scene.set_imagekey_animspeed(1,1,30);
  scene.set_imagekey_animspeed(0,0,60);
  scene.set_imagekey_animspeed(1,0,20);

  scene.set_imagekey_visible(0,0,1);
  scene.set_imagekey_visible(1,0,1);
  scene.set_imagekey_visible(0,1,1);
  scene.set_imagekey_visible(1,1,1);

  scene.set_cycles(0,200);
  scene.set_cycles(1,300);

  scene.set_coordinates(0,0,1,1);
  scene.set_coordinates(1,0,1,1);
  scene.set_coordinates(2,0,1,1);

  scene.set_coordinates(0,1,0,100);
  scene.set_coordinates(1,1,200,20);
  scene.set_coordinates(2,1,100,100);

  while(ret!=1)
    {
      if(keyboard::is_pushed(Escape_Key)) break;
      // Clear the screen
      screen::drawbox(0,0,320,200,0xFFFFFF);
      ret=scene.render_scene();
      screen::show();
    }

  SDL_Quit();
  return(0);
}
