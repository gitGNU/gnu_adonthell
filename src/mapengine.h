/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _MAPENGINE_H
#define _MAPENGINE_H

#include "screen.h"
#include "input.h"
#include "landmap.h"
#include "mapview.h"

class mapengine
{
 public:
  mapengine();
  ~mapengine();
  
  void set_mapview_schedule(char * s);
  void load_map(const char * fname);
  landmap * get_landmap() { return &lmap; }
  void run();
  void quit() { letsexit = true; }

  s_int8 get_state(gzFile file);
  s_int8 put_state(gzFile file);

  void mainloop();

 protected:
  landmap lmap;
  mapview mv;
  bool letsexit;
};

#endif
