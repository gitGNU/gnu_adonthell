#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "fileops.h"
#include "gfx.h"
#include "mapcharacter.h"
#include "mapevent.h"
#include "mappattern.h"
#include "mapsquare.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"

//u_int8 (*mapevent::event[256])
//  (mapcharacter*aguy,map*amap,u_int16 x,u_int16 y);

/* mapevent::mapevent()
{
  event[0]=&event_0;
  event[1]=&event_1;
  event[2]=&event_2;
  event[3]=&event_3;
  event[4]=&event_4;
  event[5]=&event_5;
  event[7]=&event_7;
  event[8]=&event_8;
  event[9]=&event_9;
  event[10]=&event_10;
  event[11]=&event_11;
  event[255]=&event_255;
}*/

void mapevent::get(FILE * file)
{
  fread(&type,sizeof(type),1,file);
  fread(&param1,sizeof(param1),1,file);
  fread(&param2,sizeof(param2),1,file);
  fread(&param3,sizeof(param3),1,file);
  fread(&param4,sizeof(param4),1,file);
  fread(&param5,sizeof(param5),1,file);
  fread(&param6,sizeof(param6),1,file);
  fread(&param7,sizeof(param7),1,file);
  fread(&param8,sizeof(param8),1,file);
  fread(&param9,sizeof(param9),1,file);
  getstringfromfile(param10,file);
  fread(&otherevent,sizeof(otherevent),1,file);
}

u_int8 mapevent::event_0(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  return(0);
}

u_int8 mapevent::event_1(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  aguy->set_movtype(0);
  if((param7)&&(aguy->get_nbr()==255)) mapengine::fade_out(amap);
  amap->leave_character(aguy->get_posx(),aguy->get_posy());
  aguy->set_posx(param1);
  aguy->set_posy(param2);
  if(param3!=255) aguy->set_framefactor(param3);
  amap->put_character(aguy->get_posx(),aguy->get_posy(),aguy);
  aguy->init_moveframe();

  if(aguy->get_nbr()==255)
    {
      amap->set_posx(param4);
      amap->set_posy(param5);
      if(param6!=255) amap->set_scrolltype(param6);
      amap->set_movtype(0);
    }
  amap->run_event(otherevent,aguy,x,y);
  if((param7)&&(aguy->get_nbr()==255)) mapengine::fade_in(amap);
  return(0);
}

u_int8 mapevent::event_2(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (amap->get_scrolltype()==1) amap->set_scrolltype(2);
      else if (amap->get_scrolltype()==2) amap->set_scrolltype(1);
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_3(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(1);
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_4(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(2);
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_5(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param7) mapengine::fade_out(amap);
      amap->~map();
      amap->load(param10);
      amap->init_for_scrolling();
      if (param7) mapengine::fade_in(amap);
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_6(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param7) mapengine::fade_out(amap);
      amap->leave_character(aguy->get_posx(),aguy->get_posy());
      amap->~map();
      amap->load(param10);
      aguy->set_posx(param1);
      aguy->set_posy(param2);
      if(param3!=255) aguy->set_framefactor(param3);
      amap->set_posx(param4);
      amap->set_posy(param5);
      if(param6!=255) amap->set_scrolltype(param6);
      amap->init_for_scrolling();
      if(param7) mapengine::fade_in(amap);
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_7(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
     /* Enable Horizontal Scrolling */
{
  if (aguy->get_nbr()==255)
    {
      if (param1)
	{
	  if(aguy->get_movtype()!=param1)
	      return(0);
	}
      amap->enable_horizontal_scrolling();
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_8(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
     /* Disable Horizontal Scrolling */
{
  if (aguy->get_nbr()==255)
    {
      if (param1)
	{
	  if(aguy->get_movtype()!=param1)
	      return(0);
	}
      amap->disable_horizontal_scrolling();
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_9(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
     /* Enable Vertical Scrolling */
{
  if (aguy->get_nbr()==255)
    {
      if (param1)
	{
	  if(aguy->get_movtype()!=param1)
	      return(0);
	}
      amap->enable_vertical_scrolling();
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_10(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
     /* Disable Vertical Scrolling */
{
  if (aguy->get_nbr()==255)
    {
      if (param1)
	{
	  if(aguy->get_movtype()!=param1)
	      return(0);
	}
      amap->disable_vertical_scrolling();
    }
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_11(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  map tmap;
  if(aguy->get_nbr()!=255) return(0);
  aguy->set_movtype(0);
  mapengine::fade_out(amap);
  tmap.load(param10);
  // Launch mapengine on tmap
  //  update_mapkeyboard(amap);
  mapengine::fade_in(amap);
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

u_int8 mapevent::event_255(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_status(1);
  amap->run_event(otherevent,aguy,x,y);
  return(0);
}

void mapevent::run(mapcharacter*aguy, map*amap, u_int16 x, u_int16 y)
{
  switch(type)
    {
      case 0:event_0(aguy,amap,x,y); break;
      case 1:event_1(aguy,amap,x,y); break;
      case 2:event_2(aguy,amap,x,y); break;
      case 3:event_3(aguy,amap,x,y); break;
      case 4:event_4(aguy,amap,x,y); break;
      case 5:event_5(aguy,amap,x,y); break;
      case 6:event_6(aguy,amap,x,y); break;
      case 7:event_7(aguy,amap,x,y); break;
      case 8:event_8(aguy,amap,x,y); break;
      case 9:event_9(aguy,amap,x,y); break;
      case 10:event_10(aguy,amap,x,y); break;
      case 11:event_11(aguy,amap,x,y); break;
      case 255:event_255(aguy,amap,x,y); break;
    }
}
