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

#ifndef _WIN_THEME_H_
#define _WIN_THEME_H_

#include "win_scroll.h"

class win_theme
{
  
 public:
  win_theme();
  
  win_theme(char *);
  
  win_theme(win_theme &);
  
  win_theme & operator=(win_theme &);
 
  ~win_theme();
  
  void destroy();
 
  friend class win_border;
  friend class win_background;
  friend class win_scrollbar;

 private:
  
  win_border * normal;
  
  win_border * mini;
  
  win_background * background;
  
  win_scrollbar * scrollbar;
};
#endif








