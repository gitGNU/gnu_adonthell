#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "keyboard.h"
#include "mapcharacter.h"
#include "mappattern.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"

#include "interpreter.h"
#include "array_tmpl.h"
#include "dialog.h"

dialog_engine *dlg;

void mapengine::map_engine(map*amap)
{
  amap->set_status(0);
  amap->init_for_scrolling();
  amap->win.init(80,20,150,60,WIN_NORMAL);
  //amap->win.set_text("I hope you enjoy this window system! ;) The words are cutted if they reach the end of a line, but please press Enter key:    Hopefully messages can be scrolled. Now let's go for new adventures!                      Joel"); 

  // Just a quick hack
  dlg = new dialog_engine;
  dlg->update (amap->win);
  
  amap->win.show();
  fade_in(amap);
  while(1)
    {
      update_and_show(amap);
      if (amap->get_status()) break;
      screen::show();
    }
  fade_out(amap);
}

void mapengine::update_and_show(map*amap)
{
  u_int16 i;
  for(i=0;i<screen::get_frames_to_do();i++)
    {
      dlg->update(amap->win);
      amap->update_keyboard();
      amap->update_patterns();
      amap->update_all_characters();
      amap->win.update();
    }
  amap->update_status();
  amap->draw_down();
  amap->draw_all_characters();
  amap->draw_up();
  amap->win.draw();
}

void mapengine::fade_out(map*amap) 
{
  s_int8 i;
  amap->set_status(MAP_STATUS_FADE);
  for (i=0;i<90;i+=screen::get_frames_to_do()*2) 
    {
      update_and_show(amap); 
      screen::makesquare(amap->depx,amap->depy,i);
      screen::show();
    }
  amap->set_status(MAP_STATUS_NORMAL);
}

void mapengine::fade_in(map*amap) 
{
  s_int8 i;
  amap->set_status(MAP_STATUS_FADE);
  for (i=90;i>0;i-=screen::get_frames_to_do()*2) 
    {
      update_and_show(amap); 
      screen::makesquare(amap->depx,amap->depy,i);
      screen::show();
    }
  amap->set_status(MAP_STATUS_NORMAL);
}
