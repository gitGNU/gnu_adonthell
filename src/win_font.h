/*
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
  bool table_core[WIN_NB_TABLE_CHAR];

  
  
  u_int8 height_;
  u_int8 length_; //just for space bar
 public:
    
  win_font(char *);
  win_font(win_font &);
  win_font();
  ~win_font();
  void load(char *);
  void load_font(char *);
  bool in_table(u_int16 tmp);
  image & operator[](int);
  win_font & operator=(win_font &);
  
  u_int16 height(){return height_;}
  u_int16 length(){return length_;}
  
  image * cursor;
  image * table;
};

#endif

