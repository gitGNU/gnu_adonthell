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


#include "fileops.h"
#include "mapengine.h"
#include "win_manager.h"

mapengine::mapengine()
{
  mv.resize(screen::length(),screen::height());
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

void mapengine::load_map(const char * fname)
{
  lmap.load(fname);
  //  mv.detach_map();
  mv.attach_map(&lmap);
}

void mapengine::run()
{
  screen::init_frame_counter();
  while(!letsexit)
    {
      mainloop();
      screen::show();
      if(input::has_been_pushed(SDLK_ESCAPE)) letsexit=true;
    }
}

void mapengine::mainloop()
{
    input::update();
    for(int i=0;i<screen::frames_to_do();i++)
    {
	win_manager::update ();
	lmap.update();
	mv.update();
    }
    mv.draw(0,0);
    win_manager::draw ();
}

s_int8 mapengine::get_state(gzFile file)
{
    //  char * t;
  u_int16 nbr_of,i;
  string t;

  // Load the map from the filename
  
  t = fileops::get_string2(file);

  cout << "Loading map: " << t << endl;

  load_map (t.c_str ());
  //  delete[] t;

  // Load the mapcharacters
  gzread(file,&nbr_of,sizeof(nbr_of));

  cout << "Loading " << nbr_of << " mapcharacters\n";

  for(i=0;i<nbr_of;i++)
    {
      char * t=fileops::get_string(file);
      mapcharacter * mc=(mapcharacter*)(data::characters.get(t));
      cout << "Loading " << t << endl;
      delete[] t;
      lmap.add_mapcharacter(mc);
      mc->get_state(file);
      mc->set_on_map(&lmap);
      mc->set_pos(mc->get_submap(),mc->get_posx(),mc->get_posy());
    }
  // Load the mapview state
  mv.get_state(file);
  return 0;
}

s_int8 mapengine::put_state(gzFile file)
{
  u_int16 nbr_of,i;
  // Save the map filename
  fileops::put_string(file, lmap.filename().c_str());

  // Save the mapcharacters and their status
  nbr_of=lmap.mapchar.size();
  gzwrite(file,&nbr_of,sizeof(nbr_of));
  for(i=0;i<nbr_of;i++)
    {
      fileops::put_string(file,lmap.mapchar[i]->get_name());
      lmap.mapchar[i]->put_state(file);
    }

  // Save the mapview state
  mv.put_state(file);
  return 0;
}
