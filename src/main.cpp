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
#include <unistd.h>
#include <string>
#include "types.h"
#include "input.h"
#include "gfx.h"
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"
#include "SDL.h"
#include "SDL_thread.h"
#ifdef SDL_MIXER
#include "SDL_mixer.h"
#include "audio_thread.h"
#include "audio.h"
#endif
#include "animation.h"
#include "cutscene.h"

int do_cutscene(void) {

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

  scene->load("gfxtree/cutscene/intro.cut"); // Isn't this much shorter !!

  scene->set_imagekey_anim(0,anim);
  scene->set_imagekey_anim(1,anim2);
  scene->set_imagekey_anim(2,anim3);
  scene->set_imagekey_anim(3,anim4);

  // Fade in our intro background music
#ifdef SDL_MIXER
  if (audio_in != NULL) audio_in->fade_in_background(0, 500);
#endif

  // Timing for animation is based on time from this point
  scene->initialize_timer();

  while(scene->render_scene()!=1)
  {
    if(input::is_pushed(Escape_Key)) {
#ifdef SDL_MIXER
      if (audio_in != NULL) audio_in->fade_out_background(500);
#endif
      free(scene);
      free(anim);
      free(anim2);
      free(anim3);
      free(anim4);
      free(wnd);
      return(1);
    }
    if(input::is_pushed(Enter_Key)) {
#ifdef SDL_MIXER
      if (audio_in != NULL) audio_in->fade_out_background(500);
#endif
      screen::drawbox(0,0,320,200,0x000000);
      free(scene);
      free(anim);
      free(anim2);
      free(anim3);
      free(anim4);
      free(wnd);
      return(0);
    }
    screen::show();
  }

  wnd->init(40, 25, 240, 150, WIN_NORMAL);
  wnd->show ();
  wnd->set_text ("             Welcome to Adonthell!                                                           A lot has happened since the last release; still, finalizing this current one hasn't been the easiest thing to do. Due to the constant development there always were unfinished features that delayed shipping. And once we had completed a set of features, a whole new batch was already cropping up ... .                       Now that the mapengine codebase is reaching its final state and 'role playing aspects' are implemented, we should finally be able to provide previews on a more frequent basis and with more interactive content.                    'til that happens, enjoy this release!                                                     The Adonthell Team");
  wnd->update ();
  wnd->draw ();
  screen::show ();

  while (!input::is_pushed(Escape_Key)) {
    if (input::is_pushed(Enter_Key)) {
      // Clear all stuff out of the screen buffer
      screen::drawbox(0,0,320,200,0x000000);
#ifdef SDL_MIXER
      if (audio_in != NULL) audio_in->fade_out_background(500);
#endif
      free(scene);
      free(anim);
      free(anim2);
      free(anim3);
      free(anim4);
      free(wnd);
      return(0);
    }
  }
  return(1);
}

int main(int argc, char * argv[])
{
  map * map1 = new map;
  string mapname;

  chdir(argv[1]);
  mapname=argv[1];
  mapname+=".map";
  mapengine::init(argc,argv);

  if(map1->load(mapname.data())) {printf("Error loading %s!\n",argv[1]);return(1);}

  // Flip flop between the mapengine and the cutscene
  // The 'do_cutscene' returns 1 when you hit escape
  // during the cutscene...

  // Removed because of a segfault - seems to occur when calling
  //   scene->set_imagekey_anim(2,anim3);, line 79
  // do_cutscene();

  mapengine::map_engine(map1);

  mapengine::cleanup();
  
  return(0);
}
