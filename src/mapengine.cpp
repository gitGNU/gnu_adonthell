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


#include "mapengine.h"
#include "win_manager.h"

mapengine::mapengine()
{
  lmap=NULL;
  mv.resize(320,240);
  letsexit=false;
}

mapengine::~mapengine()
{
  win_manager::destroy ();
}

void mapengine::set_mapview_schedule(char * s)
{
  mv.set_schedule(s);
}

void mapengine::use_map(landmap * lm)
{
  lmap=lm;
  mv.attach_map(lmap);
}

void mapengine::run()
{
  screen::init_frame_counter();
  while(!letsexit)
    {
      if(input::has_been_pushed(SDLK_ESCAPE)) letsexit=true;
      mainloop();
      screen::show();
    }
}

void mapengine::mainloop()
{
  input::update();
  for(int i=0;i<screen::get_frames_to_do();i++)
    {
      win_manager::update ();
      lmap->update();
      mv.update();
    }
 mv.draw(0,0);
 win_manager::draw ();
}
