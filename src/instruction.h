/*
   $Id$

   Copyright (C) 2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _instruction_h
#define _instruction_h

#include "instruction_class.h"

class mapcharacter;
class map;

#define cinstr(x) class x : public instruction  \
{                                               \
   public:                                      \
   x() : instruction() {};                      \
   void exec(mapcharacter*, map*, u_int16, u_int16); }   

cinstr(event_0);
cinstr(event_1);
cinstr(event_2);
cinstr(event_3);
cinstr(event_4);
cinstr(event_5);
cinstr(event_6);
cinstr(event_7);
cinstr(event_8);
cinstr(event_9);
cinstr(event_10);
cinstr(event_11);
cinstr(event_255);
cinstr(event_item);
cinstr(action_key);
cinstr(modif_pict);
cinstr(get_item);
cinstr(get_key);
cinstr(open_item);
cinstr(walk_flag);
cinstr(place_item);
cinstr(framefactor);

#endif
