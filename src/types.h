#ifndef _types_h
#define _types_h

// $Id$
/*
   Copyright (C) 1999 Alexandre Courbot.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   See the COPYING file for more details.

 */

#include <SDL/SDL.h>

// We should put ifdefs here to put the correct int values accordingly to the system type

#define u_int8 unsigned char
#define u_int16 unsigned short
#define u_int32 unsigned int
#define s_int8 signed char
#define s_int16 signed short
#define s_int32 signed int

/*
#define u_int8 Uint8
#define u_int16 Uint16
#define u_int32 Uint32
#define s_int8 Sint8
#define s_int16 Sint16
#define s_int32 Sint32
*/
#endif
