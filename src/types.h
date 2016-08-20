/*
   Copyright (C) 1999/2000/2001 Alexandre Courbot.
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
 */


/** @file types.h
 *  @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 *  @brief Declares some basic types.
 */ 

 
#ifndef TYPES_H__
#define TYPES_H__

#include <SDL.h>

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
typedef Uint8 u_int8;
typedef Uint16 u_int16;
typedef Uint32 u_int32;
typedef Sint8 s_int8;
typedef Sint16 s_int16;
typedef Sint32 s_int32;
*/ 
#endif
