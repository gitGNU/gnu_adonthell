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
#include "dialog_engine.h"

mapengine::mapengine()
{
  lmap=NULL;
  mv.resize(320,240);
  letsexit=false;
  th=new win_theme(win_theme::theme);
}

mapengine::~mapengine()
{
  win_manager::destroy ();
  delete th;
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

void mapengine::launch_dialog(character_base * whichcar, char * dlg_file)
{
  dialog_engine *de = new dialog_engine (whichcar, dlg_file, th, 0);
  win_manager::add (de);
  win_manager::set_focus (de);
  de->run ();
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
  for(int i=0;i<screen::frames_to_do();i++)
    {
      win_manager::update ();
      lmap->update();
      mv.update();
    }
 mv.draw(0,0);
 win_manager::draw ();
}
