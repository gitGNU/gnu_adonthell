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

#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "image.h"
#include "mapcharacter.h"
#include "mapsquare.h"

mapsquare::mapsquare()
{
  OCCUPED=0;
}

void mapsquare::get(SDL_RWops * file)
{
  SDL_RWread(file,&patternnbr,sizeof(patternnbr),1);
  SDL_RWread(file,&secpatternnbr,sizeof(secpatternnbr),1);
  SDL_RWread(file,&event,sizeof(event),1);
  SDL_RWread(file,&eventleave,sizeof(eventleave),1); 
  SDL_RWread(file,&flags,sizeof(flags),1);
  SDL_RWread(file,&alpha,sizeof(alpha),1); 
  SDL_RWread(file,&solid,sizeof(solid),1); 
}

void mapsquare::put(SDL_RWops * file)
{
  SDL_RWwrite(file,&patternnbr,sizeof(patternnbr),1);
  SDL_RWwrite(file,&secpatternnbr,sizeof(secpatternnbr),1);
  SDL_RWwrite(file,&event,sizeof(event),1);
  SDL_RWwrite(file,&eventleave,sizeof(eventleave),1); 
  SDL_RWwrite(file,&flags,sizeof(flags),1);
  SDL_RWwrite(file,&alpha,sizeof(alpha),1); 
  SDL_RWwrite(file,&solid,sizeof(solid),1); 
}

void mapsquare::put_character(mapcharacter * character)
{
  OCCUPED=character->get_nbr();
}

void mapsquare::leave_character()
{
  OCCUPED=0;
}

u_int16 mapsquare::is_occuped()
{
  return(OCCUPED);
}

u_int16 mapsquare::get_down_pattern_nbr()
{
  return(patternnbr);
}

u_int16 mapsquare::get_up_pattern_nbr()
{
  return(secpatternnbr);
}

u_int16 mapsquare::get_eventcomenbr()
{
  return(event);
}

u_int16 mapsquare::get_eventleavenbr()
{
  return(eventleave);
}

bool mapsquare::is_unreachable()
{
  return(flags&PARAM_UNREACHABLE);
}

bool mapsquare::is_others_unreachable()
{
  return(flags&PARAM_OTHERS_UNREACHABLE);
}

bool mapsquare::is_mask()
{
  return(flags&PARAM_MASK);
}

bool mapsquare::is_up()
{
  return(flags&PARAM_UP);
}

bool mapsquare::is_trans(u_int8 * alpha_res)
{
  bool res;
  res=(flags&PARAM_TRANS);
  if((res)&&(alpha_res!=NULL)) *alpha_res=alpha;
  return(res);
}

bool mapsquare::is_solid_up()
{
  return(solid&SOLID_UP);
}

bool mapsquare::is_solid_down()
{
  return(solid&SOLID_DOWN);
}

bool mapsquare::is_solid_left()
{
  return(solid&SOLID_LEFT);
}

bool mapsquare::is_solid_right()
{
  return(solid&SOLID_RIGHT);
}
