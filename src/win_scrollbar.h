/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_SCROLLBAR_H_
#define WIN_SCROLLBAR_H_

class image;
class win_base;
class win_scrolled;

class win_scrollbar
{
 public:
  image * back_bot;
  image * back_mid;
  image * back_top;
  image * bar_top;
  image * bar_bot;
  image * bar_mid;
  image * bar_flex;
  image * bar;
  image * back;
    
  win_scrollbar(win_scrollbar &);
  win_scrollbar(char *rep);
  ~win_scrollbar();
  win_scrollbar & operator=(win_scrollbar &);
  void update_scroll_bar(win_scrolled * ws);
  void load(char *);
  void update(win_base *);
  void destroy();
};

#endif



