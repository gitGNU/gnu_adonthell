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
#include "dialog_engine.h"

mapengine::mapengine()
{
  lmap=NULL;
  de=NULL;
  mv.resize(320,240);
  letsexit=false;
  th=new win_theme(win_theme::theme);
}

mapengine::~mapengine()
{
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
  de=new dialog_engine(whichcar,dlg_file,th,0);
  de->run();
  while(de->is_running && !letsexit)
    {
      if(input::has_been_pushed(SDLK_ESCAPE)) letsexit=true;
      mainloop();
      screen::show();
    }
  delete de;
  de=NULL;
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
      if(de) de->update();
      /*	  if (!ds && !de) 
		  {		
		  if (input::is_pushed(SDLK_l)) ds = new data_screen (LOAD_SCREEN);
		  if (input::is_pushed(SDLK_s)) ds = new data_screen (SAVE_SCREEN);
		  
		  if (input::is_pushed(SDLK_t)) 
		  {
		  de = new dialog_engine ((npc*)data::characters.next(), new win_theme (win_theme::theme));
		  de->run ();
		  }
		  }*/
      lmap->update();
      mv.update();
    }
  /*        if (de && !de->update ())
	    {
	    delete de;
	    de = NULL;
	    }
	    if (ds && ds->update ())
	    {
	    delete ds;
	    ds = NULL;
	    }
	    }
	    //      	screen::clear();*/
 mv.draw(0,0);
 if(de) de->draw();
 /*	if (ds) ds->draw ();
	if (de) de->draw ();*/
}
