/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_BACKGROUND_H_
#define WIN_BACKGROUND_H_

class win_theme;
class win_base;

#include "image.h"
#include "win_types.h"

class win_background
{
 public:
  
  win_background();
  
  win_background(win_base *);

  win_background(win_background &);
  
  win_background(char * rep);
  
  ~win_background();
  
  void load(char *);
  
  void update();
  
  void destroy();

  void draw(drawing_area * da =NULL);

  void set_visible_background(bool b){visible_=b;}

  void set_background(win_background &);

  void set_background(win_theme & wt);
  
  void set_brightness_background(bool b);
  
  void set_trans_background(bool b);//{trans_=b;}
  
 private:
  win_background & operator=(win_background & wb);
  
  void refresh();

  void init();
  
  image * background_template_;
  
  image * background_;
  
  image * background_brightness_;
  
  image * background_draw_;
  
  win_base * wb_;
  
  bool visible_;

  bool brightness_;

  bool trans_;
};



#endif
