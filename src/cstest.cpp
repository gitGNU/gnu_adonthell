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

#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "input.h"
#include "image.h"
#include "globals.h"

#include "SDL.h"
#include "SDL_thread.h"
#include "input.h"

#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#endif

#include "animation.h"
#include "cutscene.h"
#include "window.h"

int main(int argc, char * argv[])
{
  cutscene *scene = new cutscene();
  animation *anim = new animation();
  animation *anim2 = new animation();
  animation *anim3 = new animation();
  animation *anim4 = new animation();
  window *wnd = new window();

  screen::init_display(0);

  SDL_Thread *input_thread;

  input_thread = SDL_CreateThread(input::init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }

#ifdef SDL_MIXER
  SDL_Thread *audio_thread = NULL;

  // Create our audio manager
  audio_in = new audio();

  if ( audio_in != NULL) {
    audio_in->load_background(0, "audio/at-1.it");
    audio_in->fade_in_background(0, 500);

    audio_thread = SDL_CreateThread(audio_update, NULL);

    if ( audio_thread == NULL) {
      fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
      fprintf(stderr, "Audio will not be used\n");
      free(audio_in);
    }
  } else {
    fprintf(stderr, "Couldn't create audio thread: Can't create audio pointer\n");
    fprintf(stderr, "Audio will not be used\n");
  }
#endif

  anim->load_frame("gfxtree/cutscene/layer1.png");
  anim->loop=true;
  anim->reverse=false;
  anim->mask_on=false;
  anim->alpha_on=false;
  anim->alpha=0;

  anim2->load_frame("gfxtree/cutscene/layer2.png");
  anim2->loop=true;
  anim2->reverse=false;
  anim2->mask_on=true;
  anim2->alpha_on=false;
  anim2->alpha=0;

  anim3->load_frame("gfxtree/cutscene/layer3a.png");
  anim3->loop=true;
  anim3->reverse=false;
  anim3->mask_on=true;
  anim3->alpha_on=false;
  anim3->alpha=0;

  anim4->load_frame("gfxtree/cutscene/layer3b.png");
  anim4->loop=true;
  anim4->reverse=false;
  anim4->mask_on=true;
  anim4->alpha_on=false;
  anim4->alpha=0;

  scene->insert_keyframe(0);
  scene->insert_keyframe(1);

  // Generate a 'key' for each layer
  scene->insert_imagekey(0);
  scene->insert_imagekey(1);
  scene->insert_imagekey(2);
  scene->insert_imagekey(3);

  // Assign images to those keys
  scene->set_imagekey_anim(0,anim);
  scene->set_imagekey_anim(1,anim2);
  scene->set_imagekey_anim(2,anim3);
  scene->set_imagekey_anim(3,anim4);

  scene->set_imagekey_animspeed(0,0,100); // There's no animation
  scene->set_imagekey_animspeed(1,0,100); // So these are irrelevant
  scene->set_imagekey_animspeed(2,0,100);
  scene->set_imagekey_animspeed(3,0,100);

  scene->set_imagekey_animspeed(0,1,100); // There's no animation
  scene->set_imagekey_animspeed(1,1,100); // So these are irrelevant
  scene->set_imagekey_animspeed(2,1,100);
  scene->set_imagekey_animspeed(3,1,100);

  scene->set_imagekey_animspeed(0,2,100); // There's no animation
  scene->set_imagekey_animspeed(1,2,100); // So these are irrelevant
  scene->set_imagekey_animspeed(2,2,100);
  scene->set_imagekey_animspeed(3,2,100);

  scene->set_imagekey_animspeed(0,3,100); // There's no animation
  scene->set_imagekey_animspeed(1,3,100); // So these are irrelevant
  scene->set_imagekey_animspeed(2,3,100);
  scene->set_imagekey_animspeed(3,3,100);

  // Set each layer's visibility
  scene->set_imagekey_visible(0,0,true);
  scene->set_imagekey_visible(0,1,true);
  scene->set_imagekey_visible(0,2,true);
  scene->set_imagekey_visible(0,3,true);

  scene->set_imagekey_visible(1,0,true);
  scene->set_imagekey_visible(1,1,true);
  scene->set_imagekey_visible(1,2,true);
  scene->set_imagekey_visible(1,3,true);

  scene->set_imagekey_visible(2,0,true);
  scene->set_imagekey_visible(2,1,true);
  scene->set_imagekey_visible(2,2,true);
  scene->set_imagekey_visible(2,3,true);

  scene->set_imagekey_visible(3,0,true);
  scene->set_imagekey_visible(3,1,true);
  scene->set_imagekey_visible(3,2,true);
  scene->set_imagekey_visible(3,3,true);

//  scene->set_cycles(0,130);
//  scene->set_cycles(1,980);
//  scene->set_cycles(2,600);
//  scene->set_cycles(3,600);

  // Slowed down animation to 50% original speed
  scene->set_cycles(0,260);
  scene->set_cycles(1,1960);
  scene->set_cycles(2, 1200);
  scene->set_cycles(3, 1200);

  // Layer #1 coordinates
  scene->set_coordinates(0,0,0,0);
  scene->set_coordinates(1,0,0,0);
  scene->set_coordinates(2,0,-580,0);
  scene->set_coordinates(3,0,-580,0);

  // Layer #2 coordinates
  scene->set_coordinates(0,1,0,73);
  scene->set_coordinates(1,1,0,73);
  scene->set_coordinates(2,1,-780,73);
  scene->set_coordinates(3,1,-780,73);

  // Layer #3: Coordinates of left portion
  scene->set_coordinates(0,2,0,0);
  scene->set_coordinates(1,2,0,0);
  scene->set_coordinates(2,2,-494,0);
  scene->set_coordinates(3,2,-494,0);

  // Layer #4: Coordinates of right portion
  scene->set_coordinates(0,3,240,137);
  scene->set_coordinates(1,3,240,137);
  scene->set_coordinates(2,3,(-494 + 240),137);
  scene->set_coordinates(3,3,(-494 + 240),137);

  while(scene->render_scene()!=1)
    {
      if(input::is_pushed(Escape_Key)) break;
      // Clear the screen (why hendersa?)
      // screen::drawbox(0,0,320,200,0xFFFFFF);
      screen::show();
    }
  
  // -- start window code
  wnd->init(40, 20, 240, 160, WIN_NORMAL);
  wnd->show ();
  wnd->set_text ("                                            Adonthell");
  wnd->update ();
  wnd->draw ();
  screen::show ();

  while (!input::is_pushed(Escape_Key));
  // -- end window code
  
  // Clean up SDL :>
  fprintf(stderr, "Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);

#ifdef SDL_MIXER
  if (audio_thread != NULL) {

    if (audio_in != NULL) {
      audio_in->fade_out_background(500);
      audio_in->unload_background(0);
      SDL_KillThread(audio_thread);
      free(audio_in);
    } else {
      SDL_KillThread(audio_thread);
    }
  }
#endif

  free(anim);
  free(anim2);
  free(anim3);
  free(anim4);
  free(scene);
  free(wnd); 

  SDL_Quit();
  return(0);
}
