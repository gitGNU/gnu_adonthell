/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_BORDER_H_
#define WIN_BORDER_H_
class image;
class win_base;

class win_border
{
 public:
  image * h_border_template;
  image * v_border_template;
  image * corner_top_right;
  image * corner_top_left;
  image * corner_bottom_left;
  image * corner_bottom_right;
  image * h_border;
  image * v_border;

  win_border(win_border &);
  win_border(char *rep ,char *size=WIN_BORDER_NORMAL_SIZE);
  ~win_border();
  
  win_border & operator=(win_border &);
  void load(char *,char *);
  void update(win_base *);
  void destroy();
};
#endif


