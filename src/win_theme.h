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

class win_border;
class win_base;
class win_background;
class win_scrollbar;

class win_theme
{
 private:
  
  
 public:
  win_border * normal;
  win_border * mini;
  win_background * background;
  win_scrollbar * scrollbar;

  static char * theme;

  win_theme(char *);
  win_theme(win_theme &);
  win_theme & operator=(win_theme &);
  void destroy();
  void update(win_base * wb=NULL);
  ~win_theme();
};
#endif








