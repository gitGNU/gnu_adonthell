/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_BACKGROUND_H_
#define WIN_BACKGROUND_H_
class image;
class win_base;

class win_background
{
 public:
  image * background_template;
  image * background;
  
  win_background(win_background &);
  win_background(char * rep);
  ~win_background();
  void load(char *);
  void update(win_base *);
  win_background & operator=(win_background & wb);
};



#endif
