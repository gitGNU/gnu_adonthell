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

#ifndef WIN_WRITE_H_
#define WIN_WRITE_H_


class win_base;
class win_font;
class win_container;
class drawing_area;

class win_write : public win_base
{
  win_font * font;
  
  void write();
  
 protected:
  
  char text_result[TEXT_MAX];
  
  text_w text;
  int beg_select;
  int end_select;
  bool ok_text;
  
 
 public:
  win_write(u_int16,u_int16,u_int16,u_int16,win_font *fo,win_container *);
  ~win_write();
  void set_font(win_font *fo);
  bool is_text();//return true if seltext/lenght!=0
  char * get_text();//return seltext if is_text
  void draw();
  void update();
};

#endif












