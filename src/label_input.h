/*
   
   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _WRITE_H__
#define _WRITE_H__

#include "label.h"


class label_input : public label
{
 public:
    
  bool input_update();
  
 protected:
  void insert(const u_int16 pos, const char * letter);

  void insert_char(const u_int16 pos, const char letter);
  
  
};


#endif



