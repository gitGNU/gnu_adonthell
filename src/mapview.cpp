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

#include "mapview.h"
#include "landmap.h"

void mapview::init()
{
  length=height=d_length=d_height=currentsubmap=posx=posy=0;
  m_map=NULL;
  x=y=offx=offy=0;
#ifdef _EDIT_
  currentobj=0;
  walkimg=new image(MAPSQUARE_SIZE, MAPSQUARE_SIZE);
  for(u_int16 i=0;i<walkimg->get_length();i++)
    for(u_int16 j=0;j<walkimg->get_height();j++)
      walkimg->put_pix(i,j,0x0000FF);
  walkimg->set_alpha(110);
#endif
  da=new drawing_area();
}

mapview::mapview()
#ifdef _EDIT_
  : mapselect(0,0,1,1,0,0)
#endif
{
  init();
}

mapview::~mapview()
{
  delete da;
#ifdef _EDIT_
  delete walkimg;
#endif
}

void mapview::attach_map(landmap * m)
{
  m_map=m;
  currentsubmap=0;
#ifdef _EDIT_
  currentobj=0;
#endif
  if(m_map->nbr_of_submaps)
    {
      set_pos(0,0);
#ifdef _EDIT_
      current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	[mapselect::posy].tiles.begin();
      update_current_tile(*current_tile);
#endif
    }
#ifdef _EDIT_
  if(m_map->nbr_of_submaps)
    mapselect::resize(m_map->submap[currentsubmap]->length,
		      m_map->submap[currentsubmap]->height);
#endif  
}

void mapview::detach_map()
{
  m_map=NULL;
#ifdef _EDIT_
  mapselect::resize(0,0);
#endif
}

s_int8 mapview::set_current_submap(u_int16 sm)
{
  if(sm>=m_map->nbr_of_submaps) return -1;
  currentsubmap=sm;
#ifdef _EDIT_
  if(m_map->nbr_of_submaps)
    mapselect::resize(m_map->submap[currentsubmap]->length,
		      m_map->submap[currentsubmap]->height);
  set_pos(posx>=m_map->submap[currentsubmap]->length?
	  m_map->submap[currentsubmap]->length-1:posx,
	  posy>=m_map->submap[currentsubmap]->height?
	  m_map->submap[currentsubmap]->height-1:posy);
  current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
    [mapselect::posy].tiles.begin();
  must_upt_label_pos=true;
  must_upt_label_square=true;
#endif  
  return 0;
}

s_int8 mapview::set_pos(u_int16 x, u_int16 y)
{
  if(!m_map->nbr_of_submaps) return -1;
  if(x>=m_map->submap[currentsubmap]->length || 
     y>=m_map->submap[currentsubmap]->height) return -1;
  posx=x<ctrx?ctrx:x;
  posy=y<ctry?ctry:y;
  offx=0;
  offy=0;
  return 0;
}

void mapview::scroll_right()
{
  if(posx>=m_map->submap[currentsubmap]->length-(ctrx+1)) return;
  if(offx==MAPSQUARE_SIZE-1) { offx=0; posx++; }
  else offx++;
}

void mapview::scroll_left()
{
  if(posx<=ctrx && offx==0) return;
  if(offx==0) { offx=MAPSQUARE_SIZE-1; posx--; }
  else offx--;
}

void mapview::scroll_down()
{
  if(posy>=m_map->submap[currentsubmap]->height-(ctry+1)) return;
  if(offy==MAPSQUARE_SIZE-1) { offy=0; posy++; }
  else offy++;
}

void mapview::scroll_up()
{
  if(posy<=ctry && offy==0) return;
  if(offy==0) { offy=MAPSQUARE_SIZE-1; posy--; }
  else offy--;
}

void mapview::resize(u_int16 d_l, u_int16 d_h)
{
  d_length=d_l;
  d_height=d_h;
  length=d_length*MAPSQUARE_SIZE;
  height=d_height*MAPSQUARE_SIZE;
  ctrx=d_length/2;
  ctry=d_height/2;
  da->resize(d_l*MAPSQUARE_SIZE,d_h*MAPSQUARE_SIZE);
#ifdef _EDIT_
  mapselect::resize_view(d_length,d_height);
#endif
}


void mapview::update()
{
#ifdef _EDIT_
  mapselect::update();
#endif
}

void mapview::draw(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  static u_int16 i,j;
  static u_int16 i0,j0,ie,je;
  static list<mapsquare_tile>::iterator it;
  static landsubmap * l;
  if(!m_map) return;

  l=m_map->submap[currentsubmap];
  if(!l->length || !l->height) return;
  if(da) da->move(x,y);

  i0=posx-ctrx;
  j0=posy-ctry;
  ie=i0+d_length+(offx!=0)<l->length?i0+d_length+(offx!=0):l->length;
  je=j0+d_height+(offy!=0)<l->height?j0+d_height+(offy!=0):l->height;
  /*  i0=(posx<ctrx)?0:(posx>m_map->submap[currentsubmap].length-(ctrx+1))?
    m_map->submap[currentsubmap].length-(2*ctrx+1):posx-ctrx;
  j0=(posy<ctry)?0:(posy>m_map->submap[currentsubmap].height-(ctry+1))?
  m_map->submap[currentsubmap].height-(2*ctry+1):posy-ctry;*/

  // 1st horizontal parse to check top overflows
  // Top-left corner
  for(it=l->land[i0][j0].tiles.begin(); 
      it!=l->land[i0][j0].tiles.end() && *(it->base_tile)<*it;it++)
    if(it->x>it->base_tile->x && it->y>it->base_tile->y)
      critical_draw.push_back(*(it->base_tile));

  // Top line
  for(i=i0;i<ie && i<l->length;i++)
    for(it=l->land[i][j0].tiles.begin(); 
	it!=l->land[i][j0].tiles.end();it++)
      if(it->x==it->base_tile->x && it->y>it->base_tile->y)
	critical_draw.push_back(*(it->base_tile));
  
  // Top right corner
  for(it=l->land[ie-1][j0].tiles.begin(); 
      it!=l->land[ie-1][j0].tiles.end();it++)
    if(it->x<it->base_tile->x && it->y>it->base_tile->y)
      critical_draw.push_back(*(it->base_tile));

  // Drawing top overflowing gfx
  critical_draw.sort();
  for(it=critical_draw.begin();it!=critical_draw.end();it++)
    it->draw(this);
  critical_draw.clear();
  
  // Now drawing objects without any top or bottom overflow
  for(j=j0;j<je;j++)
    {
      // Left overflow
      for(it=l->land[i0][j].tiles.begin();it!=l->land[i0][j].tiles.end()
	    && *(it->base_tile)<=*it;it++)
	if(it->y==it->base_tile->y && it->x>it->base_tile->x)
	  it->base_tile->draw(this);
      
      // Objects which base tile is visible on the map view
      for(i=i0;i<ie;i++)
	for(it=l->land[i][j].base_begin;
	    it!=l->land[i][j].tiles.end() && *(it->base_tile)<=*it;it++)
	  it->draw(this);
      
      // Right overflow
      for(it=l->land[ie-1][j].tiles.begin();it!=l->land[ie-1][j].tiles.end();
	  it++)
	if(it->y==it->base_tile->y && it->x<it->base_tile->x)
	  it->base_tile->draw(this);      
    }

  // Last horizontal parse to check bottom overflows
  // Bottom left overflow
  if(!l->land[i0][je-1].tiles.empty())
    for(it=--l->land[i0][je-1].tiles.end();
	it->y<it->base_tile->y;it--)
      {
	if(it->x>it->base_tile->x && it->y<it->base_tile->y)
	  critical_draw.push_front(*(it->base_tile));
	if(it==l->land[i0][je-1].tiles.begin()) break;
      }
  // Bottom line
  for(i=i0;i<ie && i<l->length;i++)
    if(!l->land[i][je-1].tiles.empty())
      for(it=--l->land[i][je-1].tiles.end(); 
	  it->y<it->base_tile->y;it--)
	{
	  if(it->x==it->base_tile->x && it->y<it->base_tile->y)
	    critical_draw.push_front(*(it->base_tile));
	  if(it==l->land[i][je-1].tiles.begin()) break;
	}

  // Bottom right corner
  if(!l->land[ie-1][je-1].tiles.empty())
    for(it=--l->land[ie-1][je-1].tiles.end();
	it->y<it->base_tile->y;it--)
      {
	if(it->x<it->base_tile->x && it->y<it->base_tile->y)
	  critical_draw.push_front(*(it->base_tile));
	if(it==l->land[ie-1][je-1].tiles.begin()) break;
      }
  // Drawing bottom overflowing gfx
  critical_draw.sort();
  for(it=critical_draw.begin();it!=critical_draw.end();it++)
    it->draw(this);
  critical_draw.clear();
}

#ifdef _EDIT_

void mapview::update_label_pos()
{
  if(m_map->nbr_of_submaps)
    {
      sprintf(tmps,"Submap: %d/%d\n%d/%d  %d/%d",currentsubmap+1,
	      m_map->nbr_of_submaps,mapselect::posx+1,
	      m_map->submap[currentsubmap]->length,
	      mapselect::posy+1,m_map->submap[currentsubmap]->height);
      label_pos->set_text(tmps);
    }
  else label_pos->set_text("No submaps yet!");
  must_upt_label_pos=false;
}

void mapview::update_label_object()
{
  if(!m_map->nbr_of_patterns) sprintf(tmps,"No objects yet!");
  else sprintf(tmps,"Selected Obj:\n%d/%d",currentobj+1,
	       m_map->nbr_of_patterns);
  label_object->set_text(tmps);
  must_upt_label_object=false;
}

void mapview::update_label_square()
{
  if(!m_map->nbr_of_submaps) 
    {
      must_upt_label_square=false;
      return;
    }
  if(!m_map->submap[currentsubmap]->land[mapselect::posx][mapselect::posy].
     tiles.empty())
    {
      list<mapsquare_tile>::iterator i=
	m_map->submap[currentsubmap]->land[mapselect::posx][mapselect::posy].
	tiles.begin();
      u_int16 cpt=0;
      while(i!=current_tile)
	{
	  cpt++;
	  i++;
	}
      sprintf(tmps,"Obj. here:\n%d/%d %s",cpt+1,
	      m_map->submap[currentsubmap]->land[mapselect::posx]
	      [mapselect::posy].tiles.size(),
	      current_tile->is_base?"(Base tile)":"");
    }
  else sprintf(tmps,"No object here!");
  label_square->set_text(tmps);
  must_upt_label_square=false;
}

s_int8 mapview::resize_map(u_int16 nbr)
{
  u_int16 l,h;
  win_query * qw=new win_query(70,40,th,font,"New length:");
  char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			     makeFunctor(*this,&mapview::draw_editor));
  if(!s) return -2;
  l=atoi(s);
  if(!l) l=1;
  delete qw;
  qw=new win_query(70,40,th,font,"New height:");
  s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			     makeFunctor(*this,&mapview::draw_editor));
  if(!s) return -2;
  h=atoi(s);
  if(!h) h=1;
  delete qw;
  m_map->submap[nbr]->resize(l,h);
  return 0;
}

void mapview::add_mapobject()
{
  mapobject mobj;
  u_int16 p;
  win_query * qw=new win_query(70,40,th,font,"Load mapobject:");
  char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			     makeFunctor(*this,&mapview::draw_editor));
  if(!s) return;
  if(mobj.load(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading mapobject!");
      wi->wait_for_keypress(makeFunctor(*this,&mapview::update_editor),
			    makeFunctor(*this,&mapview::draw_editor));
      delete wi;
      delete qw;
      return;
    }
  do
    {
      char tmp[255];
      char * s2;
      sprintf(tmp,"Insert at pos(0-%d): (Default %d)",m_map->nbr_of_patterns,
	      m_map->nbr_of_patterns);
      win_query * qw2=new win_query(70,40,th,font,tmp);
      s2=qw2->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			    makeFunctor(*this,&mapview::draw_editor));
      if(!s2)
	{ 
	  delete qw2;
	  delete qw;
	  return;
	}
      if(!s2[0]) p=m_map->nbr_of_patterns;
      else p=atoi(s2);
      delete qw2;
    }
  while(p>m_map->nbr_of_patterns);
  m_map->insert_mapobject(mobj,p,s);
  delete qw;
  //  m_map->init_mapobjects();
  must_upt_label_object=true;
}

void mapview::delete_mapobject()
{
  if(m_map->nbr_of_patterns)
    {
      win_query * qw=new win_query(70,40,th,font,"Really delete object?");
      char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
				 makeFunctor(*this,&mapview::draw_editor));
      if(!s) return;
      int i=0;
      while(s[i]) 
	{
	  s[i]=toupper(s[i]);
	  i++;
	}
      if(!(strcmp(s,"Y") || strcmp(s,"YES"))) return;
      m_map->delete_mapobject(currentobj);
      if(currentobj>=m_map->nbr_of_patterns) 
	currentobj=m_map->nbr_of_patterns-1;
      if(!m_map->nbr_of_patterns) currentobj=0;
      must_upt_label_square=true;
      must_upt_label_object=true;
      if(m_map->nbr_of_submaps)
	current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	  [mapselect::posy].tiles.begin();
    }
}

void mapview::add_submap()
{
  if(!m_map->nbr_of_patterns)
    {
      win_info * wi=new win_info(70,40,th,font,"You must have objects before adding a submap!");
      wi->wait_for_keypress(makeFunctor(*this,&mapview::update_editor),
			    makeFunctor(*this,&mapview::draw_editor));
      delete wi;
      return;
    }
  m_map->add_submap();
  m_map->nbr_of_submaps--;
  if(!resize_map(m_map->nbr_of_submaps))
    {
      m_map->nbr_of_submaps++;
      set_pos(0,0);
      set_current_submap(m_map->nbr_of_submaps-1);
      must_upt_label_pos=true;
      must_upt_label_square=true;
      must_upt_label_object=true;
    }
  else
    {
      m_map->nbr_of_submaps++;
      m_map->remove_submap(m_map->nbr_of_submaps-1);
    }
  if(m_map->nbr_of_submaps)
    {
      current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	[mapselect::posy].tiles.begin();
      update_current_tile(*current_tile);
    }
}

void mapview::delete_submap()
{
  if(m_map->nbr_of_submaps)
    {
      win_query * qw=new win_query(70,40,th,font,"Really delete submap?");
      char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
				 makeFunctor(*this,&mapview::draw_editor));
      if(!s) return;
      int i=0;
      while(s[i]) 
	{
	  s[i]=toupper(s[i]);
	  i++;
	}
      if(!(strcmp(s,"Y") || strcmp(s,"YES"))) return;

      set_pos(0,0);
      m_map->remove_submap(currentsubmap);
      must_upt_label_pos=true;
      must_upt_label_square=true;
      must_upt_label_object=true;
      if(m_map->nbr_of_submaps)
	{
	  if(currentsubmap) --currentsubmap;
	  set_current_submap(currentsubmap);
	  current_tile=m_map->submap[currentsubmap]->
	    land[mapselect::posx][mapselect::posy].
	    tiles.begin();
	  update_current_tile(*current_tile);
	}
    }
}

void mapview::load_map()
{
  win_query * qw=new win_query(70,40,th,font,"Load map: (Type \"new\" for editing a new one)");
  char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			     makeFunctor(*this,&mapview::draw_editor));
  if(!s) return;
  if(strcmp(s,"new"))
    {
      if(m_map->load(s))
	{
	  win_info * wi=new win_info(70,40,th,font,"Error loading!");
	  wi->wait_for_keypress(makeFunctor(*this,&mapview::update_editor),
				makeFunctor(*this,&mapview::draw_editor));
	  delete wi;
	  delete qw;
	  return;
	}
      landmap * t=m_map;
      detach_map();
      attach_map(t);
    }
  else 
    {
      landmap * t=m_map;
      detach_map();
      t->clear();
      attach_map(t);
    }
  delete qw;
  /*  currentsubmap=0;
      currentobj=0;
      set_pos(0,0);
      set_cursor_pos(0,0);
      mapselect::resize(m_map->submap[currentsubmap]->length,
      m_map->submap[currentsubmap]->height);
      current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
      [mapselect::posy].tiles.begin();*/
  must_upt_label_pos=true;
  must_upt_label_object=true;
  must_upt_label_square=true;
}

void mapview::save_map()
{
  win_query * qw=new win_query(70,40,th,font,"Save map as:");
  char * s=qw->wait_for_text(makeFunctor(*this,&mapview::update_editor),
			     makeFunctor(*this,&mapview::draw_editor));
  if(!s) return;
  if(m_map->save(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&mapview::update_editor),
			    makeFunctor(*this,&mapview::draw_editor));
      delete wi;
      delete qw;
      return;
    }
  delete qw;
  must_upt_label_pos=true;
  must_upt_label_object=true;
  must_upt_label_square=true;
}

void mapview::update_current_tile(mapsquare_tile t)
{
  current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
    [mapselect::posy].tiles.begin();

  while(current_tile->base_tile!=t.base_tile && 
	current_tile!=m_map->submap[currentsubmap]->
	land[mapselect::posx][mapselect::posy].tiles.end())
    {
      current_tile++;
    }
  if(current_tile==m_map->submap[currentsubmap]->
     land[mapselect::posx][mapselect::posy].tiles.end())
    current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
      [mapselect::posy].tiles.begin();
}

void mapview::increase_obj_here()
{
  if(!m_map->nbr_of_submaps) return;
  if(current_tile!=--m_map->submap[currentsubmap]->land[mapselect::posx]
     [mapselect::posy].tiles.end())
    {
      current_tile++;
    }
  else 
    {
      current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	 [mapselect::posy].tiles.begin();     
    }
  must_upt_label_square=true;
}

void mapview::decrease_obj_here()
{
  if(!m_map->nbr_of_submaps) return;
  if(current_tile!=m_map->submap[currentsubmap]->land[mapselect::posx]
     [mapselect::posy].tiles.begin())
    {
      current_tile--;
    }
  else 
    {
      list<mapsquare_tile>::iterator i=--m_map->submap[currentsubmap]->land
	[mapselect::posx][mapselect::posy].tiles.begin();
      current_tile=--m_map->submap[currentsubmap]->land[mapselect::posx]
	[mapselect::posy].tiles.end();     
      while(i!=current_tile)
	{
	  i++;
	}
    }
  must_upt_label_square=true;
}

void mapview::increase_currentobj()
{
  if(currentobj<m_map->nbr_of_patterns-1) currentobj++;
  else currentobj=0;
  must_upt_label_object=true;
}

void mapview::decrease_currentobj()
{
  if(currentobj>0) currentobj--;
  else currentobj=m_map->nbr_of_patterns-1;
  must_upt_label_object=true;
}

void mapview::move_cursor_left()
{
  if(!m_map->nbr_of_submaps) return;
  mapsquare_tile t;
  t=*current_tile;
  mapselect::move_cursor_left();
  if(mapselect::posx<get_posx()-ctrx)
    set_pos(get_posx()?get_posx()-1:0,get_posy());
  update_current_tile(t);
  must_upt_label_pos=true;
  must_upt_label_square=true;
}

void mapview::move_cursor_right()
{
  if(!m_map->nbr_of_submaps) return;
  mapsquare_tile t;
  t=*current_tile;
  mapselect::move_cursor_right();
  if(mapselect::posx>get_posx()+ctrx)
    set_pos(get_posx()+1,get_posy());
  update_current_tile(t);
  must_upt_label_pos=true;
  must_upt_label_square=true;
}

void mapview::move_cursor_up()
{
  if(!m_map->nbr_of_submaps) return;
  mapsquare_tile t;
  t=*current_tile;
  mapselect::move_cursor_up();
  if(mapselect::posy<get_posy()-ctry)
    set_pos(get_posx(),get_posy()?get_posy()-1:0);
  update_current_tile(t);
  must_upt_label_pos=true;
  must_upt_label_square=true;
}

void mapview::move_cursor_down()
{
  if(!m_map->nbr_of_submaps) return;
  mapsquare_tile t;
  t=*current_tile;
  mapselect::move_cursor_down();
  if(mapselect::posy>get_posy()+ctry)
    set_pos(get_posx(),get_posy()+1);
  update_current_tile(t);
  must_upt_label_pos=true;
  must_upt_label_square=true;
}

void mapview::draw_cursor()
{
  if(!m_map) return;
  if(mapselect::cursor_blink<CURSOR_BLINK_RATE)
    m_map->pattern[currentobj]->draw(s_posx+MAPSQUARE_SIZE*(mapselect::posx-
							   mapselect::d_posx),
				    s_posy+MAPSQUARE_SIZE*(mapselect::posy-
							   mapselect::d_posy),
				    da);
    /*    screen::drawbox(s_posx+MAPSQUARE_SIZE*(posx-d_posx),
		    s_posy+MAPSQUARE_SIZE*(posy-d_posy),MAPSQUARE_SIZE,
		    MAPSQUARE_SIZE,0xFFFFFF);*/

}

void mapview::draw_walkable(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  static u_int16 i,j;
  static u_int16 i0,j0,ie,je;
  static landsubmap * l;
  if(!m_map) return;

  l=m_map->submap[currentsubmap];
  if(da) da->move(x,y);

  i0=posx-ctrx;
  j0=posy-ctry;
  ie=i0+d_length+(offx!=0)<l->length?i0+d_length+(offx!=0):l->length;
  je=j0+d_height+(offy!=0)<l->height?j0+d_height+(offy!=0):l->height;

  for(j=j0;j<je;j++)
    for(i=i0;i<ie;i++)
      if(!l->land[i][j].walkable)
	{
	  u_int16 rx, ry;
	  rx=(posx>ctrx)?i-(posx-ctrx):i;
	  ry=(posy>ctry)?j-(posy-ctry):j;
	  walkimg->draw(rx*MAPSQUARE_SIZE-offx,ry*MAPSQUARE_SIZE-offy,da);
	}
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

void mapview::update_editor()
{
  update();
  m_map->update();
}

void mapview::draw_editor()
{
  screen::drawbox(0,0,320,240,0x000000);
  if(m_map->nbr_of_submaps) 
    {
      draw(0,0,da);
      if(input::is_pushed(SDLK_k)) draw_walkable(0,0,da);
      if(m_map->nbr_of_patterns) draw_cursor();
    }
  container->draw();
  if(m_map->nbr_of_patterns) m_map->mini_pattern[currentobj]->
			       draw_free(245,75);
  if(m_map->nbr_of_submaps)
    {
      if(!m_map->submap[currentsubmap]->
	 land[mapselect::posx][mapselect::posy].tiles.empty())
	m_map->mini_pattern[current_tile->objnbr]->draw_free(245,155);
      
      if(current_tile!=m_map->submap[currentsubmap]->land[mapselect::posx]
	 [mapselect::posy].tiles.end())
	{
	  current_tile->draw_border(this);
	  current_tile->draw_base_tile(this);
	}
    }
  if(must_upt_label_pos) update_label_pos();
  if(must_upt_label_object) update_label_object();
  if(must_upt_label_square) update_label_square();
}

void mapview::update_editor_keys()
{
  if(testkey(SDLK_RIGHT))
    move_cursor_right();
  if(testkey(SDLK_LEFT))
    move_cursor_left();
  if(testkey(SDLK_UP))
    move_cursor_up();
  if(testkey(SDLK_DOWN))
    move_cursor_down();
  if(testkey(SDLK_KP_PLUS))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	increase_obj_here();
      else increase_currentobj();
    }
  if(testkey(SDLK_KP_MINUS))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	decrease_obj_here();
      else decrease_currentobj();
    }

  if(input::has_been_pushed(SDLK_SPACE))
    {
      if(m_map->nbr_of_submaps && m_map->nbr_of_patterns)
	{
	  m_map->set_square_pattern(currentsubmap,mapselect::posx,
				    mapselect::posy,currentobj);
	  update_current_tile(*current_tile);
	  must_upt_label_square=true;
	}
    }

  if(input::has_been_pushed(SDLK_r))
    {
      if(m_map->nbr_of_submaps && m_map->nbr_of_patterns &&
	 current_tile!=m_map->submap[currentsubmap]->land
	 [mapselect::posx][mapselect::posy].tiles.end())
	{
	  m_map->remove_obj_from_square(currentsubmap,current_tile);
	  current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	    [mapselect::posy].tiles.begin();
	  must_upt_label_square=true;
	}
    }
  
    if(input::has_been_pushed(SDLK_F1))
      add_mapobject();

    if(input::has_been_pushed(SDLK_F2))
      delete_mapobject();
    
    if(input::has_been_pushed(SDLK_F3))
      add_submap();

    if(input::has_been_pushed(SDLK_F4))
      delete_submap();

    if(input::has_been_pushed(SDLK_F5))
      save_map();

    if(input::has_been_pushed(SDLK_F6))
      load_map();

    if(input::has_been_pushed(SDLK_s))
      {
	if(m_map->nbr_of_submaps)
	  {
	    resize_map(currentsubmap);
	    mapselect::resize(m_map->submap[currentsubmap]->length,
			      m_map->submap[currentsubmap]->height);
	    must_upt_label_pos=true;
	    current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
	      [mapselect::posy].tiles.begin();
	  }
      }

    if(input::has_been_pushed(SDLK_HOME))
      {
	set_current_submap(currentsubmap?currentsubmap-1:
			   m_map->nbr_of_submaps-1);
      }

    if(input::has_been_pushed(SDLK_END))
      {
        set_current_submap(currentsubmap!=m_map->nbr_of_submaps-1?
			   currentsubmap+1:0);
      }
}

void mapview::update_and_draw()
{
  update_editor();
  draw_editor();
}

void mapview::editor()
{
  u_int16 i;
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);
  container=new win_container(220,12,110,216,th);
  label_pos=new win_label(5,5,100,20,th,font);
  label_object=new win_label(5,45,100,20,th,font);
  label_square=new win_label(5,125,100,20,th,font);
  container->add(label_pos);
  container->add(label_object);
  container->add(label_square);
  container->set_visible_all(true);
  must_upt_label_pos=true;
  must_upt_label_object=true;
  must_upt_label_square=true;
  set_pos(0,0);
  if(m_map->nbr_of_submaps)
    current_tile=m_map->submap[currentsubmap]->land[mapselect::posx]
      [mapselect::posy].tiles.begin();
  //  update_current_tile();
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      input::update();
      for(i=0;i<screen::frames_to_do;i++) 
	{
	  update_editor_keys();
	  update_editor();
	  //	  if(m_map) m_map->update();
	}
      draw_editor();
      screen::show();
    }
  delete container;
  delete th;
  delete font;
}

#endif
