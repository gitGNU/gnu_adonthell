/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <list>
#include <string.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_scrollbar.h"

win_scrollbar::win_scrollbar(win_scrollbar & ws)
{
  back_bot=NULL;
  back_mid=NULL;
  back_top=NULL;
  bar_top=NULL;
  bar_bot=NULL;
  bar_mid=NULL;
  bar_flex=NULL;
  bar=NULL;
  back=NULL;
  *this=ws;
}


win_scrollbar::win_scrollbar(char * rep)
{
  back_bot=NULL;
  back_mid=NULL;
  back_top=NULL;
  bar_top=NULL;
  bar_bot=NULL;
  bar_mid=NULL;
  bar_flex=NULL;
  bar=NULL;
  back=NULL;
  load(rep);
}

win_scrollbar::~win_scrollbar()
{
  destroy();
}

win_scrollbar & win_scrollbar::operator=(win_scrollbar & wb)
{
  destroy();
  bar_top=new image();
  *bar_top=*(wb.bar_top);
  bar_mid=new image();
  *bar_mid=*(wb.bar_mid);
  bar_bot=new image();
  *bar_bot=*(wb.bar_bot);
  bar_flex=new image();
  *bar_flex=*(wb.bar_flex);

  back_top=new image();
  *back_top=*(wb.back_top);
  back_mid=new image();
  *back_mid=*(wb.back_mid);
  back_bot=new image();
  *back_bot=*(wb.back_bot);

  bar=new image();
  back=new image();
  return *this;
}



void win_scrollbar::load(char * theme)
{
  destroy();
  char path[255];char tmp[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_SCROLLBAR_DIRECTORY);
  strcat(path,theme);
  
  bar=new image();
  back=new image();
  
  bar_top=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_TOP);
  bar_top->load_pnm(tmp);
  
  bar_mid=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_MID);
  bar_mid->load_pnm(tmp);
  
  bar_bot=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_BOT);
  bar_bot->load_pnm(tmp);
  
  bar_flex=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_FLEX);
  bar_flex->load_pnm(tmp);

  back_top=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_TOP);
  back_top->load_pnm(tmp);
  
  back_mid=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_MID);
  back_mid->load_pnm(tmp);
  
  back_bot=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_BOT);
  back_bot->load_pnm(tmp);

  
}

void win_scrollbar::update(win_base * wb)
{
  if(wb)
    {
      back->resize(back_mid->length(),wb->height());
      back->putbox_tile_img(back_mid);
      back->putbox_img(back_top,0,0);
      back->putbox_img(back_bot,0,wb->height()-back_bot->height());
    }
}

void win_scrollbar::destroy()
{
  if(back_bot) delete back_bot;
  if(back_top) delete back_top;
  if(back_mid) delete back_mid;
  if(bar_bot) delete bar_bot;
  if(bar_mid) delete bar_mid;
  if(bar_top) delete bar_top;
  if(bar_flex) delete bar_flex;
  if(bar) delete bar;
  if(back) delete back;
}

void win_scrollbar::update_scroll_bar(win_scrolled * ws)
{
  if(ws->height()-ws->amplitude()>bar_top->height()+bar_mid->height()+bar_bot->height())
    {
      bar->resize(bar_top->length(),ws->height()-ws->amplitude());
      bar->putbox_tile_img(bar_flex);
      bar->putbox_img(bar_top,0,0);
      bar->putbox_img(bar_bot,0,bar->height()-bar_bot->height());
      bar->putbox_img(bar_mid,0,(bar->height()-bar_mid->height())>>1);
    }
  else
    {
      bar->resize(bar_top->length(),bar_top->height()+bar_mid->height()+bar_bot->height());
      bar->putbox_img(bar_top,0,0);
      bar->putbox_img(bar_bot,0,bar->height()-bar_bot->height());
      bar->putbox_img(bar_mid,0,bar_top->height());
    }
}






