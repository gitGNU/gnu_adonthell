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

#ifndef _WIN_THEME_H_
#define _WIN_THEME_H_

#include "win_border.h"
#include "win_background.h"
#include "win_scroll.h"
#include "win_scrollbar.h"

class win_theme
{
  
 public:
  win_theme();
  
  win_theme(char *);
  
  win_theme(win_theme &);
  
  win_theme & operator=(win_theme &);
 
  ~win_theme();
  
  void destroy();
 
  static char * theme;
  
  friend class win_border;
  friend class win_background;
  friend class win_scrollbar;

 private:
  
  win_border * normal;
  
  win_border * mini;
  
  win_background * background;
  
  win_scrollbar * scrollbar;
};
#endif








