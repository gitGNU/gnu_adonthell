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

#ifndef GUI_SELECT_H_
#define GUI_SELECT_H_

#include "border.h"
#include "scroll.h"

namespace gui {
  
  class select_list : public scroll
    {
    public:
      
      select_list();
      
      bool input_update();
  
      void add(base * w);
      
      void remove(base * w);
      
      void remove_all();
      
      void set_mode(const u_int8 mode){mode_ = mode;}
      
      void set_border_select(border * border){border_select_ = border;}
      
      void set_circle(const bool  b) {circle_ = b;}
      bool is_circle(){return circle_;}
      
      void set_default();
      
      void set_default_object(const base * wb);
      
      void set_default_position(const u_int16 pos);
      
      void set_pos( const u_int8 pos) {scroll::set_pos(pos);finish_scroll_ = true;}
      
      bool update();
      
      base * get_selected_object();
      
      u_int16 get_selected_position();
      
      static const u_int8 MODE_BORDER = 0;
      
      static const u_int8 MODE_BRIGHTNESS = 1;
      
    protected:
      
      void rules(const bool b, base * wb);
      
      void update_cur_select_position();
      
      void next();
      
      void previous();
      
      void activate();
      
      u_int8 mode_;
      
      bool circle_;
      
      bool finish_scroll_;
      
      border * border_select_;
      
      lwb::iterator cur_select_;
    };
}

#endif
