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
  void init_in_table();

 public:
  
  image * table;
  u_int8 height;
  u_int8 length; //just for space bar
  bool table_core[WIN_NB_TABLE_CHAR];
  win_font();
  win_font(char *);
  ~win_font();
  void load(char *);
  bool in_table(u_int16 tmp);
};

#endif
