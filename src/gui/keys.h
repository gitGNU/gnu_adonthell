/*
   $Id$

   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#ifndef GUI_KEYS_H_
#define GUI_KEYS_H_

#include "../input.h"

namespace gui {
  class keys
    {
    public:
      
      static SDLKey KEY_ACTIVATE_ENTRY;
      
      static SDLKey KEY_UP;
      
      static SDLKey KEY_DOWN;
      
      static SDLKey KEY_PREVIOUS;
      
      static SDLKey KEY_NEXT;
      
      static SDLKey KEY_ACTIVATE;
    };
}
#endif
