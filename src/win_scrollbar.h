/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_SCROLLBAR_H_
#define WIN_SCROLLBAR_H_

#include "image.h"
#include "win_types.h"

class win_theme;
class win_scroll;

class win_scrollbar
{
 public:
  
  win_scrollbar();
  
  win_scrollbar(win_scroll * );

  win_scrollbar(win_scrollbar &);
  
  win_scrollbar(char *rep);
  
  ~win_scrollbar();
  
  void update_bar();
  
  void load(char *);
  
  void update_back();
  
  void destroy();
  
  void set_scrollbar(win_scrollbar &);
  
  void set_scrollbar(win_theme  & wt);
  
  void set_visible_scrollbar(bool b)
    {visible_=b;}
  
  void set_trans_scrollbar(bool b)
    {
      if(!bar_draw_) return;
      if((trans_=b)){ bar_draw_->set_alpha(130);back_draw_->set_alpha(130); }
      else{bar_draw_->set_alpha(255);back_draw_->set_alpha(255); }
    }
  
  void set_brightness_scrollbar(bool b){brightness_=b;refresh();}
  
  void draw(drawing_area * da= NULL);
  
  u_int16 height_bar(){if(bar_) return bar_->height();return 0;}

 private:
  
  void init();
  
  void refresh();

  win_scrollbar & operator=(win_scrollbar &);
  
  image * back_bot_;
  image * back_mid_;
  image * back_top_;
  image * bar_top_;
  image * bar_bot_;
  image * bar_mid_;
  image * bar_flex_;
  
  image * bar_;
  image * back_;
  
  image * bar_brightness_;
  image * back_brightness_;
  
  image * bar_draw_;
  image * back_draw_;

  bool visible_;
  bool brightness_;
  bool trans_;

  win_scroll * wsc_;
  
};

#endif




