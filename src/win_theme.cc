/*
   $Id$ 
 
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_background.h"
#include "win_scrollbar.h"
#include "win_theme.h"

win_theme::win_theme(char * theme)
{
  normal=new win_border(theme,WIN_BORDER_NORMAL_SIZE);
  mini=new win_border(theme,WIN_BORDER_MINI_SIZE);
  background=new win_background(theme);
  scrollbar=new win_scrollbar(theme);
} 

win_theme::win_theme(win_theme & th)
{
  normal=NULL;
  mini=NULL;
  background=NULL;
  scrollbar=NULL;
  *this=th;
}

win_theme::~win_theme()
{
  destroy();
}

win_theme & win_theme::operator=(win_theme & th)
{
  destroy();
  normal = new win_border(*(th.normal));
  mini = new win_border(*(th.mini));
  background = new win_background(*(th.background));
  scrollbar=new win_scrollbar(*(th.scrollbar));
  return *this;
}

void win_theme::destroy()
{
  if(normal)delete normal;
  if(mini) delete mini;
  if(background) delete background;
  if(scrollbar) delete scrollbar;
}

void win_theme::update(win_base * wb=NULL)
{
  if(normal) normal->update(wb);
  if(mini) mini->update(wb);
  if(background) background->update(wb);
  if(scrollbar) scrollbar->update(wb);
}










