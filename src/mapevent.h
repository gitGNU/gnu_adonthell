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

class landmap;
class mapcharacter;
class mapengine;

class mapevent
{
  u_int16 type;
  u_int16 param[10];
  char param_str[100];
  u_int8 otherevent;           // Will switch to u_int16 after the
                               // map structure is changed
  void mapevent::map_action_0(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_1(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_2(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_3(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_4(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_5(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_6(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_7(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_8(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_9(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_10(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_11(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  void mapevent::map_action_255(mapcharacter*aguy,landmap*amap,u_int16 x,u_int16 y);
  
 public:
  void get(SDL_RWops * file);
  void put(SDL_RWops * file);

  u_int8 action (mapcharacter * aguy, landmap * amap, u_int16 x, u_int16 y);
};

#endif


