/*
  (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef GUI_FONT_H_
#define GUI_FONT_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "types.h"
#include "../gfx/image.h"

namespace gui {
  
  class font{
    
  private:
    void erase();
    
    void init_in_table();
    
    bool table_core[NB_TABLE_CHAR];
    
    gfx::image * table;  
    
    u_int8 height_;
    
    u_int8 length_; //just for space bar
    
  public:
    
    font(char *);
    
    font(font &);
    
    font();
    
    ~font();
    
    void load(char *);
    
    bool in_table(u_int16 tmp);
    
    gfx::image & operator[](int);
    
    u_int16 height(){return height_;}
    
    u_int16 length(){return length_;}
    
    gfx::image * cursor;
  };
}
#endif
