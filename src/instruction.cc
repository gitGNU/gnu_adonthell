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
  amap->run_event(otherevent(),aguy,x,y);
  if((param(7))&&(aguy->get_nbr()==255)) mapengine::fade_in(amap);
}


void event_2::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (amap->get_scrolltype()==1) amap->set_scrolltype(2);
      else if (amap->get_scrolltype()==2) amap->set_scrolltype(1);
    }
  amap->run_event(otherevent(),aguy,x,y);
}

void event_3::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(1);
  amap->run_event(otherevent(),aguy,x,y);
}

void event_4::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(2);
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
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
  amap->run_event(otherevent(),aguy,x,y);
}

void event_255::exec(mapcharacter*aguy,map*amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_status(1);
  amap->run_event(otherevent(),aguy,x,y);
}
