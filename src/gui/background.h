/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_BACKGROUND_H_
#define GUI_BACKGROUND_H_

#include "../gfx/image.h"
#include "../gfx/drawing_area.h"
#include "types.h"

namespace gui {

  class theme;
  class base;


  
  class background
    {
    public:
      
      background();
      
      background(base *);
      
      background(background &);
      
      background(char * rep);
      
      ~background();
      
      void load(char *);
      
      void update();
      
      void destroy();
      
      void draw(gfx::drawing_area * da =NULL);
      
      void set_visible_background(bool b){visible_=b;}
      
      void set_background(background &);
      
      void set_background(theme & wt);
      
      void set_brightness_background(bool b);
      
      void set_trans_background(bool b);
      
    private:
      background & operator=(background & wb);
      
      void refresh();
      
      void init();
      
      gfx::image * background_template_;
      
      gfx::image * background_;
      
      gfx::image * background_brightness_;
      
      gfx::image * background_draw_;
      
      base * wb_;
      
      bool visible_;
      
      bool brightness_;
      
      bool trans_;
    };
}

#endif
