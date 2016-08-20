/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WIN_SCROLLED_H
#define WIN_SCROLLED_H

#include "win_container.h"
#include "win_scrollbar.h"

class win_scroll : public win_container, public win_scrollbar
{
  
 public:
  //constructor x,y,length,height,and a theme
  win_scroll();
  
  //return difference between the last object and the visual height, I think you don't understand, but i know you never use thisfunction just me 
  u_int16 amplitude(){return max_amplitude_;}
  
  virtual void add(win_base *);
  
  virtual void remove(win_base *);
  
  virtual void remove_all();
  
  void resize(u_int16 tl,u_int16 th);
  
  void destroy();
  
  bool draw();
  
  bool update();
  
  bool input_update();

  void set_space_between_border(u_int16 );
  
  void set_space_between_object(u_int16 );
 
  virtual void set_pos (const u_int8 pos);
    
  u_int16 cursor_y(){return cursor_y_;}
  
  void set_auto_scrollbar(bool b){auto_scrollbar_=b;}
  
  void set_auto_refresh(bool b){auto_refresh_=b;}

  void set_brightness(bool b) {win_container::set_brightness(b);set_brightness_scrollbar(b);}
    
  void set_trans(bool b) {win_container::set_trans(b); set_trans_scrollbar(b);}

  const static u_int8 PAD_DEFAULT = 5;

 protected:

  bool up();
  
  bool down();

  void find_amplitude();
  
  void update_amplitude();
  
  u_int16 max_amplitude_;
  
  u_int16 cur_amplitude_;
  
  u_int16 index_pad_;
  
  u_int16 cursor_y_;
  
  bool auto_scrollbar_;

  bool auto_refresh_;
};
#endif



