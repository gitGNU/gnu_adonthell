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

#include <string>
#include "theme.h"

using namespace gui;

theme::theme()
{
  my_border_normal = NULL;
  my_border_mini = NULL;
  my_background = NULL;
  my_scrollbar = NULL;
}

theme::theme(char * theme)
{
  std::string strtheme = std::string (theme) + "/";
  my_border_normal = new border((char *) strtheme.c_str(), BORDER_NORMAL_SIZE);
  my_border_mini = new border((char *) strtheme.c_str(), BORDER_MINI_SIZE);
  my_background = new background((char *) strtheme.c_str() );
  my_scrollbar = new scrollbar((char *) strtheme.c_str() );
} 

theme::theme(theme & th)
{
  my_border_normal=NULL;
  my_border_mini=NULL;
  my_background=NULL;
  my_scrollbar=NULL; 
  *this=th;
}

theme::~theme()
{
  destroy();
}

theme & theme::operator=(theme & th)
{
  destroy();
  my_border_normal = new border(*(th.my_border_normal));
  my_border_mini = new border(*(th.my_border_mini));
  my_background = new background(*(th.my_background));
  my_scrollbar = new scrollbar(*(th.my_scrollbar));
  return *this;
}

void theme::destroy()
{
  if(my_border_normal)delete my_border_normal;
  if(my_border_mini) delete my_border_mini;
  if(my_background) delete my_background;
  if(my_scrollbar) delete my_scrollbar;
}
