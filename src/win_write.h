/*
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
class win_label;
class win_font;
class win_theme;

class win_write : public win_label
{
 private:
  u_int8 blinkclock_;

 
 protected:
  bool ok_text_;
  bool activate_keyboard_;
  void write();
  
 public:
  win_write(s_int16,s_int16,u_int16,u_int16,win_theme *,win_font * fo);
  bool update();
 
  bool is_text(){return ok_text_;}
  void set_activate_keyboard(bool b){activate_keyboard_=b;}
  bool is_activate_keyboard(){return activate_keyboard_;}
  char * get_text_entry();

  void set_activated(bool b);

};
#endif





