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

#ifndef _WIN_FONT_H_
#define _WIN_FONT_H_

class image;

class win_font{

 private:
  void erase();


 public:
  
  image * table;
  u_int8 height;
  u_int8 lenght; //just for space bar
  
  win_font();
  win_font(char *);
  ~win_font();
  void load(char *);
};

#endif
