/*
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

class image;
class win_font;
class win_base;
class win_theme;

class win_label : public win_base
{
 protected:
  win_font * font_;
  win_font * src_font_;
  char texte_[5000];
  u_int8 texte_size_;
  bool auto_height_;
  bool auto_size_;
  
  image * template_;
  
  s_int16 word_size(u_int16 begin,u_int16 & length);
  s_int8 word_place(u_int16 cur_line_size,s_int16 w_size);
  void init_draw();
  void init_draw_surface();
 public:
  win_label(s_int16,s_int16,u_int16,u_int16,win_theme *, win_font * fo);
  void draw();
  void set_text(const char * tmp);
  void add_text(const char * tmp);
  bool is_auto_size(){return auto_size_;}
  bool is_auto_height(){return auto_height_;}
  void set_auto_size(bool );
  void set_auto_height(bool );
  void resize(u_int16 tl,u_int16 th);
  void update();
  char * get_text(){return texte_;}
  ~win_label();
  
};
#endif















