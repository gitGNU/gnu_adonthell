#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "fileops.h"
#include "keyboard.h"
#include "gfx.h"
#include "mappattern.h"
#include "mapevent.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "window.h"
#include "map.h"
#include "audio_thread.h" // For sound effects tied to keyboard
using namespace std;

map::map()
{
  themap=NULL;
  maplong=0;
  maphaut=0;
  mapx=6;
  mapy=5;
  mapaddx=0;
  mapaddy=0;
  scrolltype=0;
  mapmovtype=0;
  scridx=0;
  mapspeeddelay=0;
  mapspeedcounter=0;
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
}

map::~map()
{
  u_int16 i;
  for(i=0;i<maplong;i++)
    free(themap[i]);
  free(themap);
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
  //  free(toplayername);
  //  for(i=0;i<amap->toplayer.nbr_of_frames;i++)
  //    free(amap->toplayer.pixmap[i]);
  //  free(amap->toplayer.pixmap);
}

u_int16 map::get_patternset_to_map(FILE * file, u_int16 startpos)
{
  u_int16 i;
  u_int16 nbr_read;
  fread(&nbr_read,sizeof(nbr_read),1,file);
  pattern=(mappattern*)realloc(pattern,
			       sizeof(mappattern)*(nbr_of_patterns+nbr_read));
  for(i=startpos;i<nbr_read+startpos;i++)
    if(pattern[i].get(file)) return(0);
  nbr_of_patternsets++;
  return(nbr_read);
}

u_int16 map::load_patternset_to_map(lstr fname, u_int16 startpos)
{
  u_int16 retvalue;
  FILE * file;
  file=fopen(fname,"r");
  if(!file)
  {
     printf("Error opening file %s",fname);
     exit(1);
  }
  patternname=(lstr*)realloc(patternname,sizeof(lstr)*(nbr_of_patternsets+1));
  strcpy(patternname[nbr_of_patternsets],fname);
  retvalue=get_patternset_to_map(file,startpos);
  fclose(file);
  return(retvalue);
}


s_int8 map::get(FILE * file)
{
  s_int8 retvalue=0;
  u_int16 i=0,j;
  lstr filename;
  
  fread(&nbr_of_patterns,sizeof(nbr_of_patterns),1,file);

  nbr_of_patternsets=0;
  
  while(i<nbr_of_patterns)
    {
      getstringfromfile(filename,file);
      j=i;
      if ((i+=load_patternset_to_map(filename,i))==j)
	{printf("Error loading patternset!\n");return(1);}
    }
  fread(&maplong,sizeof(maplong),1,file);
  fread(&maphaut,sizeof(maphaut),1,file);
  
  themap=(mapsquare**)calloc(sizeof(mapsquare*),maplong);
  for(i=0;i<maplong;i++)
    themap[i]=(mapsquare*)calloc(sizeof(mapsquare),maphaut);
  fread(&mapx,sizeof(mapx),1,file);
  fread(&mapy,sizeof(mapy),1,file);

  for(j=0;j<maphaut;j++)
    for(i=0;i<maplong;i++)
      themap[i][j].get(file);
  if(heroe.load("heroe.car")) retvalue=-2;
  heroe.get_heroe_stat(file);

  fread(&nbr_of_mapcharacters,sizeof(nbr_of_mapcharacters),1,file);
  othermapchar=(mapcharacter*)calloc(sizeof(mapcharacter),
				     nbr_of_mapcharacters);
  mapcharname=(lstr*)calloc(sizeof(lstr),nbr_of_mapcharacters);
  for(i=0;i<nbr_of_mapcharacters;i++)
    {
      getstringfromfile(mapcharname[i],file);
      if(othermapchar[i].load(mapcharname[i])) retvalue=-3;
      othermapchar[i].get_NPC_stat(file,i+1);
      
      themap[othermapchar[i].get_posx()][othermapchar[i].get_posy()].put_character(&othermapchar[i]);
    }

  /* Top layer */

  toplayer.get(file);
  fread(&toplayerspace,sizeof(toplayerspace),1,file);
  fread(&toplayerspeed,sizeof(toplayerspeed),1,file);
  fread(&toplayerparallaxspeed,sizeof(toplayerparallaxspeed),1,file);
  fread(&toplayerflags,sizeof(toplayerflags),1,file);
  /* Events */
  fread(&nbr_of_events,sizeof(nbr_of_events),1,file);
  event=(mapevent*)calloc(sizeof(mapevent),nbr_of_events+1);
  for(i=1;i<=nbr_of_events;i++)
    event[i].get(file);
  fread(&scrolltype,sizeof(scrolltype),1,file);
  status=MAP_STATUS_NORMAL;
  
  return(0);
}

s_int8 map::load(char * fname)
{
  FILE * file;
  u_int8 retvalue;
  file=fopen(fname,"r"); 
  if(!file) return(-1);
  retvalue=get(file);
  fclose(file);
  return(retvalue);
}

void map::init_for_scrolling()
{
  u_int16 i;
  // init_events();
  H_SCROLL_DISABLED=0;
  V_SCROLL_DISABLED=0;
  srandom(nbr_of_mapcharacters);
  i=0;
  for (i=0;i<nbr_of_mapcharacters;i++)
    {
      themap[othermapchar[i].get_posx()]
	[othermapchar[i].get_posy()].put_character(&othermapchar[i]);
      othermapchar[i].init_moveframe();
      othermapchar[i].update_frame();
    }
  themap[heroe.get_posx()][heroe.get_posy()].put_character(&heroe);
  heroe.init_moveframe();
  heroe.set_speeddelay(1);
  heroe.update_frame();
  //  inittoplayer(amap);
}

void map::follow(mapcharacter*aguy)
{
  if((aguy->get_posx()<6)||(aguy->get_posx()>=get_lenght()-7)
      ||(aguy->get_posy()<5)||(aguy->get_posy()>=get_height()-6)) return;
  mapspeeddelay=aguy->get_speeddelay();
  mapmovtype=aguy->get_movtype();
}

void map::center_on(mapcharacter*aguy)
{
  if((aguy->get_posx()>5)&&(aguy->get_posx()<get_lenght()-7)&&(!H_SCROLL_DISABLED))
    {
      mapx=aguy->get_posx();
      mapaddx=aguy->get_addx();
    }
  else
    {
      if(!H_SCROLL_DISABLED) mapx=(aguy->get_posx()<6)?6:get_lenght()-7;
      mapaddx=0;
    }
  if((aguy->get_posy()>4)&&(aguy->get_posy()<get_height()-6)&&(!V_SCROLL_DISABLED))
    {
      mapy=aguy->get_posy();
      mapaddy=aguy->get_addy();
    }
  else 
    {
      if(!V_SCROLL_DISABLED) mapy=(aguy->get_posy()<5)?5:get_height()-6;
      mapaddy=0;
    }
}

void map::setspeeddelay(u_int8 sd)
{
  mapspeeddelay=sd;
}

void map::update_status()
{
  if (status==MAP_STATUS_QUIT) 
    {
      heroe.set_movtype(0);
      //      toplayerflags&=(65535-TOP_LAYER_PARALLAX_MOVE);
    }
}

void map::update_patterns()
{
  u_int16 j;
  for (j=0;j<nbr_of_patterns;j++)
    pattern[j].update();
}

void map::update_pos()
{
  if(scrolltype==1)
    {
      /*      mapfollow(&heroe,amap);*/
      center_on(&heroe);
    }
}

void map::update_all_characters()
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

void map::update_keyboard()
{
  if(status==MAP_STATUS_FADE) return;
  keyboard::update_keyboard();
  if (keyboard::is_pushed(Escape_Key)) status=MAP_STATUS_QUIT;
  if (heroe.get_scridx()) return;
#ifdef SDL
  if((heroe.get_movtype()==RIGHT&&!keyboard::is_pushed(275))||
     (heroe.get_movtype()==LEFT&&!keyboard::is_pushed(276))||
     (heroe.get_movtype()==UP&&!keyboard::is_pushed(273))||
     (heroe.get_movtype()==DOWN&&!keyboard::is_pushed(274)))
    heroe.set_movtype(0);
  if(keyboard::is_pushed(275)&&heroe.get_movtype()==0)
    heroe.set_movtype(RIGHT);
  if(keyboard::is_pushed(276)&&heroe.get_movtype()==0)
    heroe.set_movtype(LEFT);
  if(keyboard::is_pushed(273)&&heroe.get_movtype()==0)
    heroe.set_movtype(UP);
  if(keyboard::is_pushed(274)&&heroe.get_movtype()==0)
    heroe.set_movtype(DOWN);
#else
  if((heroe.get_movtype()==RIGHT&&!keyboard::is_pushed(Right_Key))||
     (heroe.get_movtype()==LEFT&&!keyboard::is_pushed(Left_Key))||
     (heroe.get_movtype()==UP&&!keyboard::is_pushed(Up_Key))||
     (heroe.get_movtype()==DOWN&&!keyboard::is_pushed(Down_Key)))
    heroe.set_movtype(0);
  if(keyboard::is_pushed(Right_Key)&&heroe.get_movtype()==0) 
    heroe.set_movtype(RIGHT);
  if(keyboard::is_pushed(Left_Key)&&heroe.get_movtype()==0) 
    heroe.set_movtype(LEFT);
  if(keyboard::is_pushed(Up_Key)&&heroe.get_movtype()==0) 
    heroe.set_movtype(UP);
  if(keyboard::is_pushed(Down_Key)&&heroe.get_movtype()==0) 
    heroe.set_movtype(DOWN);
#endif

#ifdef SDL_MIXER
// Sound effects
  if(keyboard::is_pushed(49)) audio_in->play_wave(1,0); // '1' Key
  if(keyboard::is_pushed(50)) audio_in->play_wave(1,1); // '2' Key
#endif
}

bool map::is_ready()
{
  if (mapspeedcounter==mapspeeddelay)
    {
      mapspeedcounter=0;
      return 1;
    }else return 0;
}


void map::drawdownsquare(int x, int y, mapsquare * msqr)
{
  static u_int8 alpha;
  if((msqr->is_up())&&(!(msqr->is_mask()))
    &&(!(msqr->is_trans(NULL)))) return;
  pattern[msqr->get_down_pattern_nbr()].putbox(x,y);
  if(msqr->is_up()) return;
  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_trans(x,y,alpha);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_trans(x,y,alpha);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask(x,y);
      return;
    }
}

void map::drawupsquare(int x, int y, mapsquare * msqr)
{
  static u_int8 alpha;
  if(!(msqr->is_up())) return;

  if((msqr->is_mask())&&(msqr->is_trans(&alpha)))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask_trans(x,y,alpha);
      return;
    }
  if(msqr->is_trans(&alpha))
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_trans(x,y,alpha);
      return;
    }
  if(msqr->is_mask())
    {
      pattern[msqr->get_up_pattern_nbr()].putbox_mask(x,y);
      return;
    }
  pattern[msqr->get_up_pattern_nbr()].putbox(x,y);
}

void map::drawdownsquarepart(int x, int y, int w, int h, mapsquare * msqr, 
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

void map::draw_down()
{
  int i,j;
  if((mapaddx==0)&&(mapaddy==0))
    {
      for(i=0;i<13;i++)
	for(j=0;j<11;j++)
	  drawdownsquare((i*16)+depx,
			 (j*16)+depy,
			 &themap[mapx-6+i][mapy-5+j]);
    }
  else
    {
      if(mapaddx) for(j=0;j<11;j++)
	{
	  drawdownsquarepart(depx,
			     (j*16)+depy,
			     16-mapaddx,16,
			     &themap[mapx-6][mapy-5+j],
			     mapaddx,0);
	  
	  for(i=1;i<13;i++)
	    drawdownsquare((i*16)+depx-mapaddx,
			   (j*16)+depy-mapaddy,
			   &themap[mapx-6+i][mapy-5+j]);
	  
	  drawdownsquarepart(depx+(13*16)-mapaddx,
			     (j*16)+depy,
			     mapaddx,16,
			     &themap[mapx+7][mapy-5+j],0,0);
	}
      if(mapaddy) for(i=0;i<13;i++)
	{
	  drawdownsquarepart((i*16)+depx,depy,16,16-mapaddy,
			     &themap[mapx-6+i][mapy-5],0,mapaddy);
		  
	  for(j=1;j<11;j++)
	    drawdownsquare((i*16)+depx-mapaddx,(j*16)+depy-mapaddy,
			   &themap[mapx-6+i][mapy-5+j]);
	  
	  drawdownsquarepart((i*16)+depx,
			     depy+(11*16)-mapaddy,
			     16,mapaddy,
			     &themap[mapx-6+i][mapy+6],0,0);
	}
    }
}

void map::draw_up()
{
  int i,j;
  if((mapaddx==0)&&(mapaddy==0))
    {
      for(i=0;i<13;i++)
	for(j=0;j<11;j++)
	  drawupsquare((i*16)+depx,(j*16)+depy,
		       &themap[mapx-6+i][mapy-5+j]);
    }
  else
    {
      if (mapaddx) for(j=0;j<11;j++)
	{
	  drawupsquarepart(depx,(j*16)+depy,16-mapaddx,16,
			   &themap[mapx-6][mapy-5+j],mapaddx,0);
	  
	  for(i=1;i<13;i++)
	    drawupsquare((i*16)+depx-mapaddx,
			 (j*16)+depy-mapaddy,
			 &themap[mapx-6+i][mapy-5+j]);
	  
	  drawupsquarepart(depx+(13*16)-mapaddx,
			   (j*16)+depy,
			   mapaddx,16,
			   &themap[mapx+7][mapy-5+j],
			   0,0);
	}      
      if(mapaddy) for(i=0;i<13;i++)
	{
	  drawupsquarepart((i*16)+depx,
			   depy,
			   16,
			   16-mapaddy,
			   &themap[mapx-6+i][mapy-5],
			   0,mapaddy);
	  
	  for(j=1;j<11;j++)
	    drawupsquare((i*16)+depx-mapaddx,
			 (j*16)+depy-mapaddy,
			 &themap[mapx-6+i][mapy-5+j]);
	  
	  drawupsquarepart((i*16)+depx,
			   depy+(11*16)-mapaddy,
			   16,mapaddy,
			   &themap[mapx-6+i][mapy+6],
			   0,0);
	}
    }
}

void map::draw_character(mapcharacter * aguy)
{ 
  s_int16 drawx=depx+((96+(aguy->get_posx()*16)+aguy->get_addx()-
			    ((mapx*16)+mapaddx)));
  
  s_int16 drawy=depy+((80+(aguy->get_posy()*16)+aguy->get_addy()-
		       ((mapy*16)+mapaddy)));
  s_int16 adrawx=drawx,adrawy=drawy,asizex=16,asizey=16,adepx=0,adepy=0;
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
    }
}

void map::draw_all_characters()
{
  u_int16 i;
  for(i=0;i<nbr_of_mapcharacters;i++)
    draw_character(&othermapchar[i]);
  draw_character(&heroe);
}

bool map::is_unreachable(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_unreachable());
}
  
bool map::is_others_unreachable(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_others_unreachable());
}

u_int16 map::get_lenght()
{
  return(maplong);
}

u_int16 map::get_height()
{
  return(maphaut);
}

u_int8 map::get_status()
{
  return(status);
}

u_int8 map::get_scrolltype()
{
  return(scrolltype);
}

void map::set_status(u_int8 st)
{
  status=st;
}

void map::put_character(u_int16 x, u_int16 y, mapcharacter * aguy)
{
  themap[x][y].put_character(aguy);
}

void map::leave_character(u_int16 x, u_int16 y)
{
  themap[x][y].leave_character();
}

u_int16 map::is_occuped(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_occuped());
}

bool map::is_solid_up(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_solid_up());
}

bool map::is_solid_down(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_solid_down());
}

bool map::is_solid_left(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_solid_left());
}

bool map::is_solid_right(u_int16 x, u_int16 y)
{
  return(themap[x][y].is_solid_right());
}

void map::set_posx(u_int16 x)
{
  mapx=x;
}

void map::set_posy(u_int16 y)
{
  mapy=y;
}

void map::set_movtype(u_int16 mt)
{
  mapmovtype=mt;
}

void map::set_scrolltype(u_int16 st)
{
  scrolltype=st;
}

void map::enable_horizontal_scrolling()
{
  H_SCROLL_DISABLED=0;
}

void map::disable_horizontal_scrolling()
{
  H_SCROLL_DISABLED=1;
}

void map::enable_vertical_scrolling()
{
  V_SCROLL_DISABLED=0;
}

void map::disable_vertical_scrolling()
{
  V_SCROLL_DISABLED=1;
}

u_int16 map::get_square_eventcomenbr(u_int16 x, u_int16 y)
{
  return(themap[x][y].get_eventcomenbr());
}

u_int16 map::get_square_eventleavenbr(u_int16 x, u_int16 y)
{
  return(themap[x][y].get_eventleavenbr());
}

void map::launch_event(mapcharacter * aguy, u_int16 nbr)
{
  while(nbr!=0)
  {
    event[nbr].run(aguy,this,mapx,mapy);
    nbr=event[nbr].otherevent();
  }
}

void map::run_event(u_int16 nbr, mapcharacter * aguy, u_int16 x, u_int16 y)
{
  while(nbr!=0)
  {
    event[nbr].run(aguy,this,x,y);
    nbr=event[nbr].otherevent();
  }
}
