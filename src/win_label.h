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

#ifndef WIN_LABEL_H_
#define WIN_LABEL_H_

//#include "win_base.h"
//#include "win_types.h"

class win_font;
class image;
class win_cursor;
class win_select;

class win_label : public win_base
{
 protected:
  win_font * font;
  char texte[WIN_TEXT_MAX_LENGTH];
  u_int16 size_texte;
  bool auto_height;
  bool auto_size;
  
  image * tmp_image;
  image * tmp_draw;
  //image * cursor;

  void draw_cursor();
 
  
  
 public:
  
  void init_draw();
  
  u_int16 cur_text_pos; //cur position of the text
  u_int16 last_text_pos; //derniere position of the text

  // u_int16 x_pad_l; //x start for the text
  win_label(s_int16,s_int16,u_int16,u_int16,win_font * fo,win_container *);
  
  ~win_label();
  void set_text(const char * tmp);
  bool add_text(const char *tmp); //return true if text added
  void clear_text();
  char * get_text();//get full text of the label
  
  void set_font(win_font * tmp);  
  void set_auto_size(bool tmp);
  void set_auto_height(bool tmp);
  

  void update();
  void draw();
  void resize(u_int16 l,u_int16 h);
  void set_select_mode(u_int8 tmp);

  //void set_cursor(win_cursor * tmp);
  void draw_text(u_int16);//position to begin text

  // void attach_select(win_select *);
  // void dettach_select();
};
#endif







