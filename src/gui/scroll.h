/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_SCROLL_H_
#define GUI_SCROLL_H_

#include "../input.h"
#include "base.h"
#include "container.h"
#include "scrollbar.h"

namespace gui {
  
  class scroll : public container, public scrollbar
    {
      
    public:
      //constructor x,y,length,height,and a theme
      scroll();
      
      //return difference between the last object and the visual height, I think you don't understand, but i know you never use thisfunction just me 
      u_int16 amplitude(){return max_amplitude_;}
      
      /**
       * Add an objet
       */
      virtual void add(base *);
      
      /**
       * Remove an objet
       */
      virtual void remove(base *);
      
      /**
       * remove all 
       */
      virtual void remove_all();
      
      /**
       * resize the scroll 
       */
      void resize(u_int16 tl,u_int16 th);
      
      /**
       * destroy 
       */
      void destroy();
      
      /**
       * draw the objet
       */
      bool draw();
      
      /**
       * update
       */
      bool update();

      /**
       * input update
       */
      bool input_update();
      
      /**
       * define space between border
       */
      void set_space_between_border(u_int16 );
      
      /**
       * define space between objet
       */
      void set_space_between_object(u_int16 );
      
      /**
       * Set pos ??
       */
      virtual void set_pos (const u_int8 pos);
      
      /**
       * return position of the cursor ??
       */
      u_int16 cursor_y(){return cursor_y_;}
      

      /**
       * Define autoscrollbar 
       */
      void set_auto_scrollbar(bool b){auto_scrollbar_=b;}
      
      /**
       * Define autorefresh
       */
      void set_auto_refresh(bool b){auto_refresh_=b;}
      
      /**
       * Define if this objet is brightness
       */
      void set_brightness(bool b) {container::set_brightness(b);set_brightness_scrollbar(b);}
      
      /**
       * Define if this objet is transluency
       */
      void set_trans(bool b) {container::set_trans(b); set_trans_scrollbar(b);}
      
      /**
       * Define the pad when scroll
       */
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
}
#endif
  


