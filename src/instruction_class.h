/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "types.h"
#include <vector>
#include <string.h>
#include <stdio.h>

class map;
class mapcharacter;
class mapitem;
class mapevent;

class instruction
{
   u_int16 type;
   u_int16 param_val[9];
   char string_val[30];
   u_int8 otherevent_val;

  public:
  instruction();
  instruction(const int &, char *);
  virtual void exec(mapcharacter*, map*, u_int16, u_int16)=0;
  u_int16 param(int);
  //u_int16 otherevent();
  char* string();
  void get(FILE*);
  void put(FILE*);

  friend istream& operator>> (istream&, instruction&);
  friend ostream& operator<< (ostream&, instruction&);
  friend int loaditems (char*, 
          mapitem **, 
          mapevent **,
          u_int16*);
};
