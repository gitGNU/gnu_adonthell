/*
  (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WIN_FONT_H_
#define _WIN_FONT_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>

#if __GNUG__ > 2
#include <ext/hash_map>
using __gnu_cxx::hash_map;
#else
#include <hash_map>
#endif

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
  
  //win_font & operator=(win_font &);
  
  u_int16 height(){return height_;}
  
  u_int16 length(){return length_;}
  
  image * cursor;
  
};

#endif

