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
#include "character.h"
#include "landmap.h"
#include "mapview.h"
#include "dialog_engine.h"

class mapengine
{
 public:
  mapengine();
  ~mapengine();
  
  void set_mapview_schedule(char * s);
  void use_map(landmap * lm);
  landmap * get_landmap() { return lmap; }
  void launch_dialog(character_base * whichcar, char * dlg_file);
  void run();

  void mainloop();

 protected:
  landmap * lmap;
  mapview mv;
  dialog_engine * de;
  win_theme * th;
  bool letsexit;
};

#endif
