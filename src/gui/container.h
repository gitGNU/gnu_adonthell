/*
   $Id$

   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_CONTAINER_H_
#define GUI_CONTAINER_H_

#include<list>
#include "../gfx/drawing_area.h"
#include "base.h"

namespace gui {
  
  typedef std::list<base*> lwb;


  /**
   * Define a container. A container is able to contains an infinity of objet derived from base objet.
   */
  class container : public base
    {
    public:
      
      /**
       * Constructor
       */
      container();
      
      /**
       * Move the container.
       */
      void move(s_int16, s_int16);
      
      /**
       * Resize the container
       */
      void resize(u_int16, u_int16);
      
      /**
       * Add base objet
       */
      virtual void add(base *);
      
      /**
       * Remove an objet of the container
       */
      virtual void remove(base *);
      
      /**
       * Remove all objet
       */
      virtual void remove_all();
      
      virtual void destroy();
      
      virtual ~container();
      
      virtual bool update();
      
      virtual bool input_update();
      
      virtual bool draw();
      
      void set_visible_all(bool b);
      
      virtual void set_brightness(bool b);
      
      virtual void set_trans(bool b);
      
      virtual void set_space_with_border(u_int16 b){space_with_border_=b;update_layout();}
      
      virtual void set_space_with_object(u_int16 o){space_with_object_=o;update_layout();}
      
      u_int16 space_with_border(){return space_with_border_;}
      
      u_int16 space_with_object(){return space_with_object_;}
      
      void set_layout(u_int8 l){layout_=l;update_layout();}
      
      void set_focus_object(base * f);
      
      base * focus_object(){return focus_object_;}
      
      const static u_int8 SPACE_WITH_BORDER = 10;
      const static u_int8 SPACE_WITH_OBJECT = 10;
      
      const static u_int8 LIST_LAYOUT = 1;
      const static u_int8 NO_LAYOUT = 0;
      
    protected:
      
      void update_position();
      void update_layout();
      
      u_int16 space_with_object_;
      u_int16 space_with_border_;
      u_int8 layout_;
      
      lwb list_wb_;
      
      base * focus_object_;
      
    };
}
#endif








