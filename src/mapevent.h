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

#ifndef _mapevent_h
#define _mapevent_h

#include <vector>

class instruction;
class map;
class mapcharacter;
class mapengine;

class mapevent : vector<instruction*>
{
  //  static u_int8 (*mapevent::event[256])
  //    (mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);

   vector<instruction*> program;
   u_int8 otherevent_val;           // Will switch to u_int16 after the
                                    // map structure is changed

 public:
  void get(FILE * file);
  void run(mapcharacter*aguy, map*amap, u_int16 x, u_int16 y);
  void set_otherevent(const u_int16&);
  u_int16 otherevent();
};

#endif
