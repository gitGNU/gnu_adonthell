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

#include "win_theme.h"

win_theme::win_theme()
{
  normal = NULL;

  mini = NULL;

  background = NULL;

  scrollbar = NULL;
}

win_theme::win_theme(char * theme)
{
    string strtheme = string (theme) + "/";

  normal=new win_border((char *) strtheme.c_str(), WIN_BORDER_NORMAL_SIZE);
  
  mini=new win_border((char *) strtheme.c_str(), WIN_BORDER_MINI_SIZE);
  
  background=new win_background((char *) strtheme.c_str() );
  
  scrollbar=new win_scrollbar((char *) strtheme.c_str() );
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







