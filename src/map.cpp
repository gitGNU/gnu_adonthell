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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "fileops.h"
#include "input.h"
#include "image.h"
#include "mappattern.h"
#include "mapevent.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "window.h"
#include "map.h"
#ifdef SDL_MIXER
#include "audio.h"
#endif
using namespace std;

landmap::landmap()
{
  maparea=NULL;
  length=0;
  height=0;
  posx=6;
  posy=5;
  addx=0;
  addy=0;
  scrolltype=0;
  movtype=0;
  scridx=0;
  speeddelay=0;
  speedcounter=0;
  ATTACKABLE=0;
  nbr_of_patternsets=0;
  nbr_of_patterns=0;
  pattern=NULL;
  nbr_of_mapcharacters=0;
  othermapchar=NULL;
  nbr_of_events=0;
  event=NULL;
  //  toplayerposx=NULL;
  //  toplayerposy=NULL;
  //  toplayerflags=0;
  //  toplayernbr=0;
  //  toplayerspace=0;
  //  toplayerdecayh=0;
  //  toplayerdecayv=0;
  //  toplayerspeed=0;
  //  toplayerspeedcounter=0;
  //  toplayerhlimit=0;
  //  toplayervlimit=0;
  patternname=NULL;
  mapcharname=NULL;
  //  amap->toplayername=0;
  draw_zone=new drawing_area(56,12,13*16,11*16);
}
/*
map::~map()
{
  u_int16 i;
  for(i=0;i<length;i++)
    free(maparea[i]);
  free(maparea);
  for(i=0;i<nbr_of_patterns;i++)
    pattern[i].~mappattern();
  free(pattern);
  free(patternname);
  for(i=0;i<nbr_of_mapcharacters;i++)
    othermapchar[i].~mapcharacter();
  heroe.~mapcharacter();
  free(othermapchar);
  free(mapcharname);
  free(event);
  delete draw_zone;
  //  free(toplayername);
  //  for(i=0;i<amap->toplayer.nbr_of_frames;i++)
  //    free(amap->toplayer.pixmap[i]);
  //  free(amap->toplayer.pixmap);
}
*/
u_int16 landmap::get_patternset_to_map(SDL_RWops * file, u_int16 startpos)
{
  u_int16 i;
  u_int16 nbr_read;
  SDL_RWread(file,&nbr_read,sizeof(nbr_read),1);
  nbr_of_patterns+=nbr_read;
  pattern=(mappattern*)realloc(pattern,
			       sizeof(mappattern)*(nbr_of_patterns+nbr_read));
  for(i=startpos;i<nbr_read+startpos;i++)
    if(pattern[i].get(file)) return(0);
  return(nbr_read);
}

u_int16 landmap::load_patternset_to_map(lstr fname, u_int16 startpos)
{
  u_int16 retvalue;
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"r");
  if(!file)
  {
     printf("Error opening file %s",fname);
     exit(1);
  }
  retvalue=get_patternset_to_map(file,startpos);
  SDL_RWclose(file);
  return(retvalue);
}

s_int8 landmap::get(SDL_RWops * file)
{
  u_int16 i=0,j;
  u_int16 rev_number=0;      // Revision number

  SDL_RWread(file,&rev_number,sizeof(rev_number),1);
  SDL_RWread(file,&nbr_of_patternsets,sizeof(nbr_of_patternsets),1);

  patternname=(lstr*)calloc(sizeof(lstr),nbr_of_patternsets);
  for(i=0;i<nbr_of_patternsets;i++)
    getstringfromfile(patternname[i],file);

  SDL_RWread(file,&length,sizeof(length),1);
  SDL_RWread(file,&height,sizeof(height),1);

  SDL_RWread(file,&posx,sizeof(posx),1);
  SDL_RWread(file,&posy,sizeof(posy),1);

  maparea=(mapsquare**)calloc(sizeof(mapsquare*),length);
  for(i=0;i<length;i++)
    maparea[i]=(mapsquare*)calloc(sizeof(mapsquare),height);

  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      maparea[i][j].get(file);

  heroe.get_heroe_stat(file);

  SDL_RWread(file,&nbr_of_mapcharacters,sizeof(nbr_of_mapcharacters),1);
  mapcharname=(lstr*)calloc(sizeof(lstr),nbr_of_mapcharacters);
  othermapchar=(mapcharacter*)calloc(sizeof(mapcharacter),
				     nbr_of_mapcharacters);
  for(i=0;i<nbr_of_mapcharacters;i++)
    {
      getstringfromfile(mapcharname[i],file);
      othermapchar[i].get_NPC_stat(file,i+1);
    }

  SDL_RWread(file,&nbr_of_events,sizeof(nbr_of_events),1);
  event=(mapevent*)calloc(sizeof(mapevent),nbr_of_events+1);
  for(i=1;i<=nbr_of_events;i++)
    event[i].get(file);
  SDL_RWread(file,&scrolltype,sizeof(scrolltype),1);

  status=MAP_STATUS_NORMAL;
  return(0);
}

s_int8 landmap::load_map_data()
{
  u_int16 i,oldnbr;

  // load patterns
  for(i=0;i<nbr_of_patternsets;i++)
    {
      oldnbr=nbr_of_patterns;
      load_patternset_to_map(patternname[i],nbr_of_patterns);
      if(oldnbr==nbr_of_patterns)
	{printf("Error loading patternset!\n");return(1);}
    }
  // load characters
  heroe.load("heroe.car");
  for(i=0;i<nbr_of_mapcharacters;i++)
    othermapchar[i].load(mapcharname[i]);
  
  return(0);
}

s_int8 landmap::put(SDL_RWops * file)
{
  u_int16 i=0,j;
  u_int16 rev_number=0;      // Revision number

  SDL_RWwrite(file,&rev_number,sizeof(rev_number),1);
  SDL_RWwrite(file,&nbr_of_patternsets,sizeof(nbr_of_patternsets),1);

  for(i=0;i<nbr_of_patternsets;i++)
    putstringtofile(patternname[i],file);

  SDL_RWwrite(file,&length,sizeof(length),1);
  SDL_RWwrite(file,&height,sizeof(height),1);

  SDL_RWwrite(file,&posx,sizeof(posx),1);
  SDL_RWwrite(file,&posy,sizeof(posy),1);

  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      maparea[i][j].put(file);

  heroe.put_heroe_stat(file);

  SDL_RWwrite(file,&nbr_of_mapcharacters,sizeof(nbr_of_mapcharacters),1);

  for(i=0;i<nbr_of_mapcharacters;i++)
    {
      putstringtofile(mapcharname[i],file);
      othermapchar[i].put_NPC_stat(file);
    }

  SDL_RWwrite(file,&nbr_of_events,sizeof(nbr_of_events),1);
  for(i=1;i<=nbr_of_events;i++)
    event[i].put(file);
  SDL_RWwrite(file,&scrolltype,sizeof(scrolltype),1);

  return(0);
}

s_int8 landmap::load(const char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(-1);
  retvalue=get(file);
  SDL_RWclose(file);
  return(retvalue);
}

s_int8 landmap::save(const char * fname)
{
  SDL_RWops * file;
  u_int8 retvalue;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(-1);
  retvalue=put(file);
  SDL_RWclose(file);
  return(retvalue);
}

void landmap::init_for_scrolling()
{
  u_int16 i;
  // init_events();
  H_SCROLL_DISABLED=0;
  V_SCROLL_DISABLED=0;
  srandom(nbr_of_mapcharacters);
  i=0;
  for (i=0;i<nbr_of_mapcharacters;i++)
    {
      maparea[othermapchar[i].get_posx()]
	[othermapchar[i].get_posy()].put_character(&othermapchar[i]);
      othermapchar[i].init_moveframe();
      othermapchar[i].update_frame();
    }
  maparea[heroe.get_posx()][heroe.get_posy()].put_character(&heroe);
  heroe.init_moveframe();
  heroe.set_speeddelay(1);
  heroe.update_frame();
  //  inittoplayer(amap);
}

void landmap::follow(mapcharacter*aguy)
{
  if((aguy->get_posx()<6)||(aguy->get_posx()>=get_lenght()-7)
      ||(aguy->get_posy()<5)||(aguy->get_posy()>=get_height()-6)) return;
  speeddelay=aguy->get_speeddelay();
  movtype=aguy->get_movtype();
}

void landmap::center_on(mapcharacter*aguy)
{
  if((aguy->get_posx()>5)&&(aguy->get_posx()<get_lenght()-7)&&(!H_SCROLL_DISABLED))
    {
      posx=aguy->get_posx();
      addx=aguy->get_addx();
    }
  else
    {
      if(!H_SCROLL_DISABLED) posx=(aguy->get_posx()<6)?6:get_lenght()-7;
      addx=0;
    }
  if((aguy->get_posy()>4)&&(aguy->get_posy()<get_height()-6)&&(!V_SCROLL_DISABLED))
    {
      posy=aguy->get_posy();
      addy=aguy->get_addy();
    }
  else 
    {
      if(!V_SCROLL_DISABLED) posy=(aguy->get_posy()<5)?5:get_height()-6;
      addy=0;
    }
}

void landmap::setspeeddelay(u_int8 sd)
{
  speeddelay=sd;
}

void landmap::update_status()
{
  if (status==MAP_STATUS_QUIT) 
    {
      heroe.set_movtype(0);
      //      toplayerflags&=(65535-TOP_LAYER_PARALLAX_MOVE);
    }
}

void landmap::update_patterns()
{
  u_int16 j;
  for (j=0;j<nbr_of_patterns;j++)
    pattern[j].update();
}

void landmap::update_pos()
{
  if(scrolltype==1)
    {
      /*      mapfollow(&heroe,amap);*/
      center_on(&heroe);
    }
}

void landmap::update_all_characters()
{
  static u_int16 i;
  heroe.update_main_move(this);
  heroe.update_frame();
  heroe.update_eventleave(this);
  heroe.update_pos(this);
  update_pos();
  heroe.update_eventcome(this);
  for(i=0;i<nbr_of_mapcharacters;i++)
    othermapchar[i].update_NPC(this);
}

void landmap::update_keyboard()
{
  if(status==MAP_STATUS_FADE) return;
  if (input::is_pushed(Escape_Key)) {
    status=MAP_STATUS_QUIT;

#ifdef SDL_MIXER
    audio_in->fade_out_background(500);
#endif
  }

  if (heroe.get_scridx()) return;

  if((heroe.get_movtype()==RIGHT&&!input::is_pushed(275))||
     (heroe.get_movtype()==LEFT&&!input::is_pushed(276))||
     (heroe.get_movtype()==UP&&!input::is_pushed(273))||
     (heroe.get_movtype()==DOWN&&!input::is_pushed(274)))
    heroe.set_movtype(0);
  if(input::is_pushed(275)&&heroe.get_movtype()==0)
    heroe.set_movtype(RIGHT);
  if(input::is_pushed(276)&&heroe.get_movtype()==0)
    heroe.set_movtype(LEFT);
  if(input::is_pushed(273)&&heroe.get_movtype()==0)
    heroe.set_movtype(UP);
  if(input::is_pushed(274)&&heroe.get_movtype()==0)
    heroe.set_movtype(DOWN);

#ifdef SDL_MIXER
  // Sound effects tied to keyboard
//  if(keyboard::is_pushed(49)) audio_in->play_wave(2,0); // '1' Key
//  if(keyboard::is_pushed(50)) audio_in->play_wave(1,1); // '2' Key

  // Change background music (500 ms fade in/out, slot of new music)
//  if (keyboard::is_pushed(51)) audio_in->change_background(1, 500); // '3' Key
//  if (keyboard::is_pushed(52)) audio_in->change_background(0, 500);  // '4' Key

  // Change background music volume
//  if (keyboard::is_pushed(53)) audio_in->set_background_volume(audio_in->background_volume - 10); // '5' Key
//  if (keyboard::is_pushed(54)) audio_in->set_background_volume(audio_in->background_volume + 10); // '6' Key
#endif
}

bool landmap::is_ready()
{
  if (speedcounter==speeddelay)
    {
      speedcounter=0;
      return 1;
    }else return 0;
}


void landmap::drawdownsquare(int x, int y, mapsquare * msqr)
{
  static u_int8 alpha;
  if((msqr->is_up())&&(!(msqr->is_mask()))
    &&(!(msqr->is_trans(NULL)))) return;
  pattern[msqr->get_down_pattern_nbr()].putbox(x,y,draw_zone);
  if(msqr->is_up()) return;
  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_trans(x,y,alpha,
							    draw_zone);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_trans(x,y,alpha,draw_zone);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask(x,y,draw_zone);
      return;
    }
}

void landmap::drawupsquare(int x, int y, mapsquare * msqr)
{
  static u_int8 alpha;
  if(!(msqr->is_up())) return;

  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_trans(x,y,alpha,
							    draw_zone);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_trans(x,y,alpha,draw_zone);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask(x,y,draw_zone);
      return;
    }
  pattern[msqr->get_up_pattern_nbr()].putbox(x,y,draw_zone);
}

/*void map::drawdownsquarepart(int x, int y, int w, int h, mapsquare * msqr, 
			     int xo, int yo)
{
  static u_int8 alpha;
  if((msqr->is_up())&&(!(msqr->is_mask()))
    &&(!(msqr->is_trans(NULL)))) return;
  
  pattern[msqr->get_down_pattern_nbr()].putbox_part(x,y,w,h,xo,yo);
  if(msqr->is_up()) return;
  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_part_trans
	(x,y,w,h,xo,yo,alpha);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_part_trans
	(x,y,w,h,xo,yo,alpha);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_part(x,y,w,h,xo,yo);
      return;
    }
}

void map::drawupsquarepart(int x, int y, int w, int h, mapsquare * msqr, 
			   int xo, int yo)
{
  static u_int8 alpha;
  if(!(msqr->is_up())) return;
  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_part_trans
	(x,y,w,h,xo,yo,alpha);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_part_trans
	(x,y,w,h,xo,yo,alpha);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_part(x,y,w,h,xo,yo);
      return;
    }
  pattern[msqr->get_up_pattern_nbr()].putbox_part(x,y,w,h,xo,yo);
}
*/
void landmap::draw_down(u_int16 depx=56, u_int16 depy=12, 
		    u_int16 length=13, u_int16 height=11)
{
  u_int16 i,j;
  if(addx) length++;
  if(addy) height++;
  for(i=0;i<length;i++)
    for(j=0;j<height;j++)
      drawdownsquare((i*16)+depx-addx,
		     (j*16)+depy-addy,
		     &maparea[posx-6+i][posy-5+j]);
  /*
  if((addx==0)&&(addy==0))
    {
      for(i=0;i<length;i++)
	for(j=0;j<height;j++)
	  drawdownsquare((i*16)+depx,
			 (j*16)+depy,
			 &maparea[posx-6+i][posy-5+j]);
    }
  else
    {
      if(addx) for(j=0;j<height;j++)
	{
	  drawdownsquarepart(depx,
			     (j*16)+depy,
			     16-addx,16,
			     &maparea[posx-6][posy-5+j],
			     addx,0);
	  
	  for(i=1;i<length;i++)
	    drawdownsquare((i*16)+depx-addx,
			   (j*16)+depy-addy,
			   &maparea[posx-6+i][posy-5+j]);
	  
	  drawdownsquarepart(depx+(length*16)-addx,
			     (j*16)+depy,
			     addx,16,
			     &maparea[posx+7][posy-5+j],0,0);
	}
      if(addy) for(i=0;i<length;i++)
	{
	  drawdownsquarepart((i*16)+depx,depy,16,16-addy,
			     &maparea[posx-6+i][posy-5],0,addy);
		  
	  for(j=1;j<height;j++)
	    drawdownsquare((i*16)+depx-addx,(j*16)+depy-addy,
			   &maparea[posx-6+i][posy-5+j]);
	  
	  drawdownsquarepart((i*16)+depx,
			     depy+(height*16)-addy,
			     16,addy,
			     &maparea[posx-6+i][posy+6],0,0);
	}
	}*/
}

void landmap::draw_up(u_int16 depx=56, u_int16 depy=12,
		  u_int16 length=13, u_int16 height=11)
{
  u_int16 i,j;
  for(i=0;i<length+1;i++)
    for(j=0;j<height+1;j++)
      drawupsquare((i*16)+depx-addx,
		   (j*16)+depy-addy,
		   &maparea[posx-6+i][posy-5+j]);
  /*
  if((addx==0)&&(addy==0))
    {
      for(i=0;i<length;i++)
	for(j=0;j<height;j++)
	  drawupsquare((i*16)+depx,(j*16)+depy,
		       &maparea[posx-6+i][posy-5+j]);
    }
  else
    {
      if (addx) for(j=0;j<height;j++)
	{
	  drawupsquarepart(depx,(j*16)+depy,16-addx,16,
			   &maparea[posx-6][posy-5+j],addx,0);
	  
	  for(i=1;i<length;i++)
	    drawupsquare((i*16)+depx-addx,
			 (j*16)+depy-addy,
			 &maparea[posx-6+i][posy-5+j]);
	  
	  drawupsquarepart(depx+(length*16)-addx,
			   (j*16)+depy,
			   addx,16,
			   &maparea[posx+7][posy-5+j],
			   0,0);
	}      
      if(addy) for(i=0;i<length;i++)
	{
	  drawupsquarepart((i*16)+depx,
			   depy,
			   16,
			   16-addy,
			   &maparea[posx-6+i][posy-5],
			   0,addy);
	  
	  for(j=1;j<height;j++)
	    drawupsquare((i*16)+depx-addx,
			 (j*16)+depy-addy,
			 &maparea[posx-6+i][posy-5+j]);
	  
	  drawupsquarepart((i*16)+depx,
			   depy+(height*16)-addy,
			   16,addy,
			   &maparea[posx-6+i][posy+6],
			   0,0);
	}
	}*/
}

void landmap::draw_character(mapcharacter * aguy, u_int16 depx=56, u_int16 depy=12)
{ 
  s_int16 drawx=depx+((96+(aguy->get_posx()*16)+aguy->get_addx()-
		       ((posx*16)+addx)));
  
  s_int16 drawy=depy+((80+(aguy->get_posy()*16)+aguy->get_addy()-
		       ((posy*16)+addy)));
  aguy->draw(drawx,drawy,draw_zone);
  
  /*  s_int16 adrawx=drawx,adrawy=drawy,asizex=16,asizey=16,adepx=0,adepy=0;
    u_int16 drawable=0;
  
  if ((drawx<(depx+(208)))&&
      (drawx>depx-(16))&&
      (drawy<(depy+(176)))&&
      (drawy>depy-(16))) 
    {                             
      if ((drawx<=(depx+(192)))&&
	  (drawx>=depx)&&
	  (drawy<=(depy+(160)))&&
	  (drawy>=depy))
	  aguy->draw(drawx,drawy);
      else
	{
	  if (drawx>(depx+(192)))                    
	    {
	      adrawx=drawx;
	      asizex=depx+(208)-drawx;
	      adepx=0;
	      drawable=1;
	    }
	  if (drawx<depx)                          
	    {
	      adrawx=depx;
	      asizex=(16)-(depx-drawx);
	      adepx=depx-drawx;
	      drawable=1;
	    }
	  
	  if (drawy>(depy+(160)))                   
	    {
	      adrawy=drawy;
	      asizey=depy+(176)-drawy;
	      adepy=0;
	      drawable=1;
	    }
	  
	  if (drawy<depy)                          
	    {
	      adrawy=depy;
	      asizey=(16)-(depy-drawy);
	      adepy=depy-drawy;
	      drawable=1;
	    }
	  if(drawable) aguy->draw_part(adrawx,adrawy,asizex,asizey,
				       adepx,adepy);
	}
	}*/
}

void landmap::draw_all_characters(u_int16 depx=56, u_int16 depy=12)
{
  u_int16 i;
  for(i=0;i<nbr_of_mapcharacters;i++)
    draw_character(&othermapchar[i],depx,depy);
  draw_character(&heroe);
}

bool landmap::is_unreachable(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_unreachable());
}
  
bool landmap::is_others_unreachable(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_others_unreachable());
}

u_int16 landmap::get_lenght()
{
  return(length);
}

u_int16 landmap::get_height()
{
  return(height);
}

u_int8 landmap::get_status()
{
  return(status);
}

u_int8 landmap::get_scrolltype()
{
  return(scrolltype);
}

void landmap::set_status(u_int8 st)
{
  status=st;
}

void landmap::put_character(u_int16 x, u_int16 y, mapcharacter * aguy)
{
  maparea[x][y].put_character(aguy);
}

void landmap::leave_character(u_int16 x, u_int16 y)
{
  maparea[x][y].leave_character();
}

u_int16 landmap::is_occuped(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_occuped());
}

bool landmap::is_solid_up(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_solid_up());
}

bool landmap::is_solid_down(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_solid_down());
}

bool landmap::is_solid_left(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_solid_left());
}

bool landmap::is_solid_right(u_int16 x, u_int16 y)
{
  return(maparea[x][y].is_solid_right());
}

void landmap::set_posx(u_int16 x)
{
  posx=x;
}

void landmap::set_posy(u_int16 y)
{
  posy=y;
}

void landmap::set_movtype(u_int16 mt)
{
  movtype=mt;
}

void landmap::set_scrolltype(u_int16 st)
{
  scrolltype=st;
}

void landmap::enable_horizontal_scrolling()
{
  H_SCROLL_DISABLED=0;
}

void landmap::disable_horizontal_scrolling()
{
  H_SCROLL_DISABLED=1;
}

void landmap::enable_vertical_scrolling()
{
  V_SCROLL_DISABLED=0;
}

void landmap::disable_vertical_scrolling()
{
  V_SCROLL_DISABLED=1;
}

u_int16 landmap::get_square_eventcomenbr(u_int16 x, u_int16 y)
{
  return(maparea[x][y].get_eventcomenbr());
}

u_int16 landmap::get_square_eventleavenbr(u_int16 x, u_int16 y)
{
  return(maparea[x][y].get_eventleavenbr());
}

void landmap::launch_event(mapcharacter * aguy, u_int16 nbr)
{
  u_int16 ev=nbr;
  while(ev!=0)
      ev=event[ev].action(aguy,this,posx,posy);
}

void landmap::run_event(u_int16 nbr, mapcharacter * aguy, u_int16 x, u_int16 y)
{
  u_int16 ev=nbr;
  while(ev!=0)
      ev=event[ev].action(aguy,this,x,y);
}
