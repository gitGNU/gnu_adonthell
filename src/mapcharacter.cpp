/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "mapcharacter.h"

void mapcharacter::init()
{
  anim.resize(NBR_MOVES);
  for(u_int16 i=0; i<NBR_MOVES; i++)
    anim[i]=NULL;
  current_move=STAND_NORTH;
}

void mapcharacter::clear()
{

}

mapcharacter::mapcharacter()
{
  init();
}

mapcharacter::~mapcharacter()
{
  clear();
}

s_int8 mapcharacter::get(gzFile file)
{
  return 0;
}

s_int8 mapcharacter::load(const char * fname)
{
  return 0;
}

void mapcharacter::update()
{
  anim[current_move]->update();
}

void mapcharacter::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  anim[current_move]->draw(x,y,da_opt);
}

#ifdef _EDIT_
// New inserting method, much faster: the animation pointed by an must
// however be kept in memory, and will be deleted by destructor.

void mapcharacter::insert_anim(animation * an, c_moves pos)
{
  if(anim[pos]) delete anim[pos];
  anim[pos]=an;
}
#endif
