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
