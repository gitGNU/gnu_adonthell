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

#include <string>

void * read_pnm(SDL_RWops * file, u_int16 * length, u_int16 * height);
void pnmput(SDL_RWops * file, void * image, u_int16 length, u_int16 height);
int write_pnm(char * filename, void * image, u_int16 length, u_int16 height);
