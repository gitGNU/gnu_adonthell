/*
   $Id$
 
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
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"
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

int do_cutscene(void) {

  // Fade in our intro background music
  audio_in->fade_in_background(0, 500);

  cutscene *scene = new cutscene();
  animation *anim = new animation();
  animation *anim2 = new animation();
  animation *anim3 = new animation();
  animation *anim4 = new animation();
  window *wnd = new window();

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

  scene->set_cycles(0,130);
  scene->set_cycles(1,980);
  scene->set_cycles(2,600);
  scene->set_cycles(3,600);

  // Slowed down animation to 50% original speed
//  scene->set_cycles(0,260);
//  scene->set_cycles(1,1960);
//  scene->set_cycles(2, 1200);
//  scene->set_cycles(3, 1200);

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
    if(keyboard::is_pushed(Escape_Key)) return(1);
    if(keyboard::is_pushed(Enter_Key)) {
      audio_in->fade_out_background(500);
      screen::drawbox(0,0,320,200,0x000000);
      return(0);
    }
    screen::show();
  }

  wnd->init(40, 20, 240, 160, WIN_NORMAL);
  wnd->show ();
  wnd->set_text ("             Welcome to Adonthell!                                                           A lot has happened since the last release and yet it was not easy to get this one on the way. The biggest problem is the constant development going on. At each given point in time there had been halfway completed features, and we always said: \"Let's just finish that one first\", but when it was done, other things worth adding were already in progress ...            Now that the mapengine codebase is reaching its final state and 'role playing'-aspects are rolled in, we should be able to provide previews on a more frequent basis and with more interactive content.                               'til that happens, enjoy this release!                                                     The Adonthell Team");
  wnd->update ();
  wnd->draw ();
  screen::show ();

  while (!keyboard::is_pushed(Escape_Key)) {
    if (keyboard::is_pushed(Enter_Key)) {
      // Clear all stuff out of the screen buffer
      screen::drawbox(0,0,320,200,0x000000);
      audio_in->fade_out_background(500);
      return(0);
    }
  }
  return(1);
}

int main(int argc, char * argv[])
{
  map * map1 = new map;
  if((argc!=2)&&(argc!=3)) {printf("Usage: %s mapname\n",argv[0]); exit(0);}
  if(argc==3)
    {
      if(!strcmp(argv[2],"-m1")) screen::init_display(1);
      if(!strcmp(argv[2],"-m0")) screen::init_display(0);
  }
  else screen::init_display(0);
  if(map1->load(argv[1])) {printf("Error loading %s!\n",argv[1]);return(1);}

  SDL_Thread *input_thread;

  input_thread = SDL_CreateThread(keyboard_init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }

#ifdef SDL_MIXER
  SDL_Thread *audio_thread;

  audio_init();
  audio_thread = SDL_CreateThread(audio_update, NULL);
  if ( audio_thread == NULL) {
     fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
     fprintf(stderr, "Audio will not be used\n");
  }
#endif

  // Flip flop between the mapengine and the cutscene
  // The 'do_cutscene' returns 1 when you hit escape
  // during the cutscene...
  while (do_cutscene() != 1) {
    if (map1 == NULL) {
      map1 = new map();
      map1->load(argv[1]);
    }
    audio_in->fade_in_background(1,500);
    mapengine::map_engine(map1);
    free(map1);
    map1 = NULL;
  }

  fprintf(stderr, "Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);

#ifdef SDL_MIXER
  if (audio_thread != NULL) {
    SDL_KillThread(audio_thread);
    audio_cleanup();
  }
#endif
  //  delete map1;
  SDL_Quit();
return(0);
}