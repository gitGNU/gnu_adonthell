#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "gfx.h"
#include "mapcharacter.h"
#include "mapsquare.h"

mapsquare::mapsquare()
{
  OCCUPED=0;
}

void mapsquare::get(FILE * file)
{
  fread(&patternnbr,sizeof(patternnbr),1,file);
  fread(&secpatternnbr,sizeof(secpatternnbr),1,file);
  fread(&event,sizeof(event),1,file);
  fread(&eventleave,sizeof(eventleave),1,file); 
  fread(&flags,sizeof(flags),1,file);
  fread(&alpha,sizeof(alpha),1,file); 
  fread(&solid,sizeof(solid),1,file); 
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
