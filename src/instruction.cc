#include <iostream.h>
#include <fstream.h>
#include <vector>
#include "mapcharacter.h"
#include "map.h"
#include "mapengine.h"
#include "instruction.h"

void event_0::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
}

void event_1::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  aguy->set_movtype(0);
  if((param(7))&&(aguy->get_nbr()==255)) mapengine::fade_out(amap);
  amap->leave_character(aguy->get_posx(),aguy->get_posy());
  aguy->set_posx(param(1));
  aguy->set_posy(param(2));
  if(param(3)!=255) aguy->set_framefactor(param(3));
  amap->put_character(aguy->get_posx(),aguy->get_posy(),aguy);
  aguy->init_moveframe();
  if(aguy->get_nbr()==255)
    {
      amap->set_posx(param(4));
      amap->set_posy(param(5));
      if(param(6)!=255) amap->set_scrolltype(param(6));
      amap->set_movtype(0);
    }
  if((param(7))&&(aguy->get_nbr()==255)) mapengine::fade_in(amap);
}


void event_2::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (amap->get_scrolltype()==1) amap->set_scrolltype(2);
      else if (amap->get_scrolltype()==2) amap->set_scrolltype(1);
    }
}

void event_3::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(1);
}

void event_4::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(2);
}

void event_5::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param(7)) mapengine::fade_out(amap);
      amap->~map();
      amap->load(string());
      amap->init_for_scrolling();
      if (param(7)) mapengine::fade_in(amap);
    }
}

void event_6::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param(7)) mapengine::fade_out(amap);
      amap->leave_character(aguy->get_posx(),aguy->get_posy());
      amap->~map();
      amap->load(string());
      aguy->set_posx(param(1));
      aguy->set_posy(param(2));
      if(param(3)!=255) aguy->set_framefactor(param(3));
      amap->set_posx(param(4));
      amap->set_posy(param(5));
      if(param(6)!=255) amap->set_scrolltype(param(6));
      amap->init_for_scrolling();
      if(param(7)) mapengine::fade_in(amap);
    }
}

void event_7::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
//      Enable Horizontal Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param(1))
	{
	  if(aguy->get_movtype()!=param(1))
	      return;
	}
      amap->enable_horizontal_scrolling();
    }
}

void event_8::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
//      Disable Horizontal Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param(1))
	{
	  if(aguy->get_movtype()!=param(1))
	      return;
	}
      amap->disable_horizontal_scrolling();
    }
}

void event_9::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
//      Enable Vertical Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param(1))
	{
	  if(aguy->get_movtype()!=param(1))
	      return;
	}
      amap->enable_vertical_scrolling();
    }
}

void event_10::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
//     Disable Vertical Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param(1))
	{
	  if(aguy->get_movtype()!=param(1))
	      return;
	}
      amap->disable_vertical_scrolling();
    }
}

void event_11::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  map tmap;
  if(aguy->get_nbr()!=255) return;
  aguy->set_movtype(0);
  mapengine::fade_out(amap);
  tmap.load(string());
  // Launch mapengine on tmap
  //  update_mapkeyboard(amap);
  mapengine::fade_in(amap);
}

void event_255::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_status(1);
}
/*
void event_item::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
// This events occurs when the hero walks on an object
{
    int ev_walk;

    ev_walk = amap->items[amap->themap[x][y].item].event_walk;
    if (amap->themap[x][y].item == 0)
    {
        printf ("Error: event_object function called where there is no object\n");
        exit (1);
    }
    if (ev_walk == 0)
        return;
    //printf("Calling event %i of type %i\n",ev_walk,amap->event[ev_walk].type);
    event[ev_walk].run();
} */

void action_key::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void modif_pict::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void get_item::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void get_key::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void open_item::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void walk_flag::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void place_item::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
}

void framefactor::exec (mapcharacter * aguy, map * amap, u_int16 x, u_int16 y)
{
    // changes the frame factor to the one specified as param1
    if (param(1) != 255)
        aguy->set_framefactor(param(1));
}
