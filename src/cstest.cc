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

#include "SDL.h"
#include "SDL_thread.h"
#include "keyboard.h"

#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#endif
#include "animation.h"
#include "cutscene.h"

int main(int argc, char * argv[])
{
  cutscene scene;
  animation anim,anim2,anim3;
  u_int8 ret=0;

  screen::init_display(0);

  SDL_Thread *input_thread;

   input_thread = SDL_CreateThread((void*)keyboard_init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }

  anim.load_frame("layer1.png");
  anim.loop=true;
  anim.reverse=false;
  anim.mask_on=false;
  anim.alpha_on=false;
  anim.alpha=0;

  anim2.load_frame("layer2.png");
  anim2.loop=true;
  anim2.reverse=false;
  anim2.mask_on=true;
  anim2.alpha_on=false;
  anim2.alpha=0;

  anim3.load_frame("layer3.png");
  anim3.loop=true;
  anim3.reverse=false;
  anim3.mask_on=true;
  anim3.alpha_on=false;
  anim3.alpha=0;

  scene.insert_keyframe(0);
  scene.insert_keyframe(0);

  scene.insert_imagekey(0);
  scene.insert_imagekey(0);

  scene.set_imagekey_anim(0,&anim);
  scene.set_imagekey_anim(1,&anim2);
  scene.set_imagekey_anim(2,&anim3);

  scene.set_imagekey_animspeed(0,0,100); // There's no animation
  scene.set_imagekey_animspeed(1,0,100); // So these are irrelevant
  scene.set_imagekey_animspeed(2,0,100);
  scene.set_imagekey_animspeed(3,0,100);

  scene.set_imagekey_animspeed(0,1,100); // There's no animation
  scene.set_imagekey_animspeed(1,1,100); // So these are irrelevant
  scene.set_imagekey_animspeed(2,1,100);
  scene.set_imagekey_animspeed(3,1,100);

  scene.set_imagekey_animspeed(0,2,100); // There's no animation
  scene.set_imagekey_animspeed(1,2,100); // So these are irrelevant
  scene.set_imagekey_animspeed(2,2,100);
  scene.set_imagekey_animspeed(3,2,100);

  scene.set_imagekey_visible(0,0,1);
  scene.set_imagekey_visible(0,1,1);
  scene.set_imagekey_visible(0,2,1);

  scene.set_imagekey_visible(1,0,1);
  scene.set_imagekey_visible(1,1,1);
  scene.set_imagekey_visible(1,2,1);

  scene.set_imagekey_visible(2,0,1);
  scene.set_imagekey_visible(2,1,1);
  scene.set_imagekey_visible(2,2,1);

  scene.set_cycles(0,130);
  scene.set_cycles(1,980);
  scene.set_cycles(2,600);

  scene.set_coordinates(0,0,0,0);
  scene.set_coordinates(1,0,0,0);
  scene.set_coordinates(2,0,-580,0);
  scene.set_coordinates(3,0,-580,0);

  scene.set_coordinates(0,1,0,0);
  scene.set_coordinates(1,1,0,0);
  scene.set_coordinates(2,1,-780,0);
  scene.set_coordinates(3,1,-780,0);

  scene.set_coordinates(0,2,0,0);
  scene.set_coordinates(1,2,0,0);
  scene.set_coordinates(2,2,-494,0);
  scene.set_coordinates(3,2,-494,0);
  
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
