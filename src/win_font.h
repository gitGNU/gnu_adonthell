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

#ifndef _WIN_FONT_H_
#define _WIN_FONT_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "str_hash.h"
#include "win_types.h"
#include "image.h"

using namespace std;

class win_font{

 protected:
  void erase();
  
  hash_map<u_int16, image*> glyphs;
  
  u_int8 height_;
  u_int8 length_; //just for space bar
 
 public:
  
  win_font(char *);
  
  win_font(win_font &);
  
  win_font();
  
  virtual ~win_font();
  
  void load(char *);
  
  //  void load_font(char *);
  
  virtual bool in_table(u_int16 tmp);
  
  virtual image & operator[](u_int16);
  
  virtual s_int8 kerning(const u_int16 & char1, const u_int16 & char2)
  {
	  return 0;
  }

  //win_font & operator=(win_font &);
  
  u_int16 height(){return height_;}
  
  u_int16 length(){return length_;}
  
  image * cursor;
  
};

#endif

