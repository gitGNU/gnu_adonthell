/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_SCROLLBAR_H_
#define GUI_SCROLLBAR_H_

#include "../gfx/image.h"
#include "../gfx/drawing_area.h"
#include "types.h"

namespace gui {

  class theme;
  class scroll;

  class scrollbar
    {
    public:
      
      scrollbar();
      
      scrollbar(scroll * );
      
      scrollbar(scrollbar &);
      
      scrollbar(char *rep);
      
      ~scrollbar();
      
      void update_bar();
      
      void load(char *);
      
      void update_back();
      
      void destroy();
      
      void set_scrollbar(scrollbar &);
      
      void set_scrollbar(theme  & wt);
      
      void set_visible_scrollbar(bool b) {visible_=b;}
      
      void set_trans_scrollbar(bool b)
	{
	  if(!bar_draw_) return;
	  if(trans_=b){ bar_draw_->set_alpha(130);back_draw_->set_alpha(130); }
	  else{bar_draw_->set_alpha(255);back_draw_->set_alpha(255); }
	}
      
      void set_brightness_scrollbar(bool b){brightness_=b;refresh();}
      
      void draw(gfx::drawing_area * da= NULL);
      
      u_int16 height_bar(){if(bar_) return bar_->height();return 0;}
      
    private:
      
      void init();
      
      void refresh();
      
      scrollbar & operator=(scrollbar &);
      
      gfx::image * back_bot_;
      gfx::image * back_mid_;
      gfx::image * back_top_;
      gfx::image * bar_top_;
      gfx::image * bar_bot_;
      gfx::image * bar_mid_;
      gfx::image * bar_flex_;
  
      gfx::image * bar_;
      gfx::image * back_;
      
      gfx::image * bar_brightness_;
      gfx::image * back_brightness_;
      
      gfx::image * bar_draw_;
      gfx::image * back_draw_;
      
      bool visible_;
      bool brightness_;
      bool trans_;
      
      scroll * wsc_;
    };
}
#endif
