// $Id$
/*
   Copyright (C) 1999/2000/2001 Alexandre Courbot.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   See the COPYING file for more details.

 */


/** @file types.h
 *  @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *  @brief Declares some basic types.
 */ 

 
#ifndef _types_h
#define _types_h 

#include <SDL/SDL.h>

// We should put ifdefs here to put the correct int values accordingly to the system type

/// 8 bits long unsigned integer
#define u_int8 unsigned char

/// 16 bits long unsigned integer
#define u_int16 unsigned short

/// 32 bits long unsigned integer
#define u_int32 unsigned int

/// 8 bits long signed integer
#define s_int8 signed char

/// 16 bits long signed integer
#define s_int16 signed short

/// 32 bits long signed integer
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
