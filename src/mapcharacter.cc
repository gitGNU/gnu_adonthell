#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "mappattern.h"
#include "window.h"
#include "map.h"

mapcharacter::mapcharacter()
{
  posx=0;
  posy=0;
  addx=0;
  addy=0;
  prevmovtype=0;
  speeddelay=0;
  speedcounter=0;
  movtype=0;
  framefactor=0;
  walkcounter=0;
  moveframe=0;
  scridx=0;
  waitcounter=0;
  nbr_of_frames=0;
  is_event=0;
  mapcharnbr=0;
}

mapcharacter::~mapcharacter()
{
  u_int16 i;
  for (i=0;i<nbr_of_frames;i++)
    frame[i].~image();
  free(frame);
  frame=NULL;
  nbr_of_frames=0;
}

s_int8 mapcharacter::get(FILE * file)
{
  s_int8 i,retvalue=0;
  walkcounter=11;
  fread(&nbr_of_frames,sizeof(nbr_of_frames),1,file);
  frame=new image[nbr_of_frames];
  for (i=0;i<nbr_of_frames;i++)
    if (!(frame[i].get(file)))retvalue=-1;
  return(retvalue);
}

s_int8 mapcharacter::load(char * fname)
{
  u_int16 retvalue;
  FILE * file;
  file=fopen(fname,"r");
  if(!file) return(1);
  else
    {
      retvalue=get(file);
      fclose(file);
      return(retvalue);
    }
}

void mapcharacter::get_heroe_stat(FILE * file)
{
  fread(&posx,sizeof(posx),1,file);
  fread(&posy,sizeof(posy),1,file);
  fread(&speeddelay,sizeof(speeddelay),1,file);
  fread(&framefactor,sizeof(framefactor),1,file);
  mapcharnbr=255;
}

void mapcharacter::get_NPC_stat(FILE * file, u_int16 nbr)
{
  fread(&posx,sizeof(posx),1,file);
  fread(&posy,sizeof(posy),1,file);
  fread(&speeddelay,sizeof(speeddelay),1,file);
  fread(&framefactor,sizeof(framefactor),1,file);
  fread(&movtype,sizeof(movtype),1,file);
  mapcharnbr=nbr;
}

u_int16 mapcharacter::mapcharacter::get_nbr()
{
  return(mapcharnbr);
}

u_int16 mapcharacter::mapcharacter::get_posx()
{
  return(posx);
}

u_int16 mapcharacter::mapcharacter::get_posy()
{
  return(posy);
}

u_int8 mapcharacter::get_addx()
{
  return(addx);
}

u_int8 mapcharacter::get_addy()
{
  return(addy);
}

u_int8 mapcharacter::get_scridx()
{
  return(scridx);
}

u_int8 mapcharacter::get_movtype()
{
  return(movtype);
}

u_int8 mapcharacter::get_speeddelay()
{
  return(speeddelay);
}

void mapcharacter::update_frame()
{
  if ((movtype!=0)&&(movtype!=5)) walkcounter++;
  if (walkcounter==(16*speeddelay))
    {
      if (moveframe==0) moveframe=1;
      else moveframe=0;
      walkcounter=0;
    }
  currentframe=framefactor+moveframe;
}

void mapcharacter::init_moveframe()
{
  moveframe=0;
  if ((posx%2)||(posy%2)) moveframe=1;
  if ((posx%2)&&(posy%2)) moveframe=0;
}

void mapcharacter::draw(u_int16 x, u_int16 y)
{
  frame[currentframe].putbox_mask(x,y);
}

void mapcharacter::draw_part(u_int16 x, u_int16 y, u_int16 bw, u_int16 bh, 
	       u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_mask_part(x,y,bw,bh,xo,yo);
}

bool mapcharacter::is_ready()
{
  return(speedcounter==0);
}

bool mapcharacter::is_speed_ready()
{
  if (speedcounter==speeddelay)
    {
      speedcounter=0;
      return 1;
    }else return 0;
}

void mapcharacter::set_speeddelay(u_int8 sd)
{
  speeddelay=sd;
}

void mapcharacter::set_movtype(u_int8 mt)
{
  movtype=mt;
}

void mapcharacter::set_framefactor(u_int8 ff)
{
  framefactor=ff;
}

void mapcharacter::set_posx(u_int16 x)
{
  posx=x;
}

void mapcharacter::set_posy(u_int16 y)
{
  posy=y;
}

void mapcharacter::update_NPC(map * amap)
{
  if ((scridx==0)&&(is_ready())) 
    {
      update_dir();
      update_move(amap);
    }
  update_eventleave(amap);
  update_frame();
  update_pos(amap);
  update_eventcome(amap);
}

void mapcharacter::update_dir()
{
  switch (movtype)
    {
    case RIGHT:
      prevmovtype=LEFT;
      break;
    case LEFT:
      prevmovtype=RIGHT;
      break;
    case UP:
      prevmovtype=DOWN;
      break;
    case DOWN:
      prevmovtype=UP;
      break;
    case 0:
      prevmovtype=0;
      waitcounter++;
      if (waitcounter==(16*speeddelay)) waitcounter=0;
      break;
    }
  if (movtype!=5) do {movtype=rand()%5;} while(movtype==prevmovtype);
  if ((prevmovtype==0)&&(waitcounter!=0)) movtype=0;
}

void mapcharacter::update_move(map*amap)
{
  switch (movtype)
    {
    case DOWN:
      if (posy==amap->get_height()-1)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx,posy+1)) {movtype=0;break;}
	  if ((amap->is_occuped(posx,posy+1))||
	      (amap->is_solid_down(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx,posy+1,this);
	  framefactor=0;
	}
      break;
    case UP:
      if (posy==0)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx,posy-1)) {movtype=0;break;}
	  if ((amap->is_occuped(posx,posy-1))||
	      (amap->is_solid_up(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx,posy-1,this);
	  framefactor=2;
	}
      break;
    case RIGHT:
      if (posx==amap->get_lenght()-1)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx+1,posy)) {movtype=0;break;}
	  if ((amap->is_occuped(posx+1,posy))||
	      (amap->is_solid_right(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx+1,posy,this);
	  framefactor=4;
	}
      break;
    case LEFT:
      if (posx==0)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx-1,posy)) {movtype=0;break;}
	  if ((amap->is_occuped(posx-1,posy))||
	      (amap->is_solid_left(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx-1,posy,this);
	  framefactor=6;
	}
      break;
    }
}

void mapcharacter::update_main_move(map*amap)
{
  if(get_scridx()) return;
  switch (movtype)
    {
    case DOWN:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posy==amap->get_height()-1)||amap->is_unreachable(posx,posy+1)||
	  (amap->is_occuped(posx,posy+1))||
	  (amap->is_solid_down(posx,posy))) movtype=0;
      else amap->put_character(posx,posy+1,this);
      framefactor=0;
      break;
    case UP:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posy==0)||amap->is_unreachable(posx,posy-1)||
	  (amap->is_occuped(posx,posy-1))||
	  (amap->is_solid_up(posx,posy))) movtype=0;
      else amap->put_character(posx,posy-1,this);
      framefactor=2;
      break;
    case RIGHT:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posx==amap->get_lenght()-1)||amap->is_unreachable(posx+1,posy)||
	  (amap->is_occuped(posx+1,posy))||
	  (amap->is_solid_right(posx,posy))) movtype=0;
      else amap->put_character(posx+1,posy,this);
      framefactor=4;
      break;
    case LEFT:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posx==0)||amap->is_unreachable(posx-1,posy)||
	  (amap->is_occuped(posx-1,posy))||
	  (amap->is_solid_left(posx,posy))) movtype=0;
      else amap->put_character(posx-1,posy,this);
      framefactor=6;
      break;
    }
}

void mapcharacter::update_pos(map*amap)
{
  if (movtype!=0) speedcounter++;
  switch(movtype)
    {
    case RIGHT:
      if (!is_speed_ready()) break;
      scridx++;
      addx++;
      if (scridx==16) 
	{
	  scridx=0; 
	  addx=0 ;
	  posx++;
	  amap->leave_character(posx-1,posy);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case LEFT:
      if (!is_speed_ready()) break;
      if(scridx==0)
	{
	  scridx=addx=16;
	  posx--;
	}
      scridx--;
      addx--;
      if (scridx==0)
	{
	  amap->leave_character(posx+1,posy);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case UP:
      if (!is_speed_ready()) break;
      if(scridx==0)
	{
	  scridx=addy=16;
	  posy--;
	}
      scridx--;
      addy--;
      if (scridx==0)
	{
	  amap->leave_character(posx,posy+1);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case DOWN:
      if (!is_speed_ready()) break;
      scridx++;
      addy++;
      if (scridx==16) 
	{
	  scridx=0; 
	  addy=0 ;
	  posy++;
	  amap->leave_character(posx,posy-1);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    }
}

void mapcharacter::update_eventleave(map*amap)
{
  if(!is_event) return;
  if(is_event==MAPCHAR_EVENT_LEFT)
    {
      is_event=0;
      amap->launch_event
	(this,amap->get_square_eventleavenbr(get_posx(),get_posy()));
      return;
    }
}

void mapcharacter::update_eventcome(map*amap)
{
  if(!is_event) return;
  if(is_event==MAPCHAR_EVENT_COME)
    {
      is_event=0;
      amap->launch_event
	(this,amap->get_square_eventcomenbr(get_posx(),get_posy()));
      return;
    }
}
