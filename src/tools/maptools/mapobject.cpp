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

#include "mapobject.h"

#ifdef _DEBUG_
u_int16 mapobject::a_d_diff;
#endif

animation_off::animation_off() : animation()
{
  xoffset=0;
  yoffset=0;
}

animation_off::~animation_off()
{
}

animation_off & animation_off::operator =(animation &a)
{
  *((animation*)this)=a;
  xoffset=yoffset=0;
  return *this;
}

animation_off & animation_off::operator =(animation_off &a)
{
  *((animation*)this)=a;
  xoffset=a.xoffset;
  yoffset=a.yoffset;
  return *this;
}

void animation_off::set_offset(s_int16 x, s_int16 y)
{
  xoffset=x;
  yoffset=y;
}

s_int8 animation_off::get(gzFile file)
{
  gzread(file,&xoffset,sizeof(xoffset));
  gzread(file,&yoffset,sizeof(yoffset));
  animation::get(file);
  return 0;
}

s_int8 animation_off::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return -1;
  retvalue=get(file);
  gzclose(file);
  return retvalue;
}

#ifdef _EDIT_
s_int8 animation_off::put(gzFile file)
{
  gzwrite(file,&xoffset,sizeof(xoffset));
  gzwrite(file,&yoffset,sizeof(yoffset));
  animation::put(file);
  return 0;
}

s_int8 animation_off::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return -1;
  retvalue=put(file);
  gzclose(file);
  return retvalue;
}
#endif

void animation_off::draw(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  animation::draw(x+xoffset,y+yoffset,da_opt);
}

void animation_off::draw_border(u_int16 x, u_int16 y, 
				drawing_area * da_opt=NULL)
{
  screen::drawbox(x+xoffset,y+yoffset,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset,y+yoffset+height,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset+length,y+yoffset,1,height+1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset,y+yoffset,1,height+1,0xFFFFFF,da_opt);
}

mapsquaretpl::mapsquaretpl()
{
  walkable=true;
}

s_int8 mapsquaretpl::get(gzFile file)
{
  gzread(file,&walkable,sizeof(walkable));
  return 0;
}

s_int8 mapsquaretpl::put(gzFile file)
{
  gzwrite(file,&walkable,sizeof(walkable));
  return 0;
}

maptpl::maptpl(u_int16 x, u_int16 y, u_int16 l, u_int16 h,
	       u_int16 d_l, u_int16 d_h) : mapselect(x,y,l,h,d_l,d_h)
{
  u_int16 i,j;
  placetpl=new (mapsquaretpl*)[length];
  for(i=0;i<length;i++) placetpl[i]=new mapsquaretpl[h];
  selimg=new image(MAPSQUARE_SIZE, MAPSQUARE_SIZE);
  selbaseimg=new image(MAPSQUARE_SIZE, MAPSQUARE_SIZE);
  for(i=0;i<selimg->get_length();i++)
    for(j=0;j<selimg->get_height();j++)
      {
	selimg->put_pix(i,j,0x0000FF);
	selbaseimg->put_pix(i,j,0x00FF00);
      }
  basex=0;
  basey=0;
  selimg->set_alpha(110);
  selbaseimg->set_alpha(110);
}

maptpl::~maptpl()
{
  u_int16 i;
  for(i=0;i<length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  delete selimg;
  delete selbaseimg;
}

maptpl& maptpl::operator =(const maptpl& mt)
{
  return *this;
}

void maptpl::resize(u_int16 l, u_int16 h)
{
  u_int16 i,j;
  mapsquaretpl ** oldplacetpl=placetpl;
  placetpl=new (mapsquaretpl*)[l];
  for(i=0;i<l;i++)
    placetpl[i]=new mapsquaretpl[h];
  for(i=0;i<l && i<length;i++)
    for(j=0;j<h && j<height;j++)
      placetpl[i][j]=oldplacetpl[i][j];
  for(i=0;i<length;i++)
    delete[] oldplacetpl[i];
  delete[] oldplacetpl;
  mapselect::resize(l,h);
}

s_int8 maptpl::get(gzFile file)
{
  u_int16 i,j;
  for(i=0;i<length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  gzread(file,&length,sizeof(length));
  gzread(file,&height,sizeof(height));
  placetpl=new (mapsquaretpl*)[length];
  for(i=0;i<length;i++)
    {
      placetpl[i]=new mapsquaretpl[height];
      for(j=0;j<height;j++)
	placetpl[i][j].get(file);
    }
   gzread(file,&basex,sizeof(basex));
   gzread(file,&basey,sizeof(basey));
  return 0;
}

s_int8 maptpl::put(gzFile file)
{
  u_int16 i,j;
  gzwrite(file,&length,sizeof(length));
  gzwrite(file,&height,sizeof(height));
  for(i=0;i<length;i++)
    for(j=0;j<height;j++)
      placetpl[i][j].put(file);
  gzwrite(file,&basex,sizeof(basex));
  gzwrite(file,&basey,sizeof(basey));
  return 0;
}

void maptpl::set_base_tile(u_int16 x, u_int16 y)
{
  basex=x;
  basey=y;
}

void maptpl::toggle_walkable()
{
  placetpl[posx][posy].walkable=placetpl[posx][posy].walkable?false:true;
}

void maptpl::draw_walkables()
{
  u_int16 i,j;
  for(i=d_posx;(i<d_posx+dl)&&(i<length);i++)
    for(j=d_posy;(j<d_posy+dh)&&(j<height);j++)
      {
	if(!placetpl[i][j].walkable)
	  selimg->draw(s_posx+((i-d_posx)*MAPSQUARE_SIZE),
		       s_posy+((j-d_posy)*MAPSQUARE_SIZE));
      }
}

void maptpl::draw_base_tile()
{
  if((basex>=d_posx && basex<d_posx+dl)&&
     (basey>=d_posy && basey<d_posy+dh))
    selbaseimg->draw(s_posx+(basex-d_posx)*MAPSQUARE_SIZE,
		     s_posy+(basey-d_posy)*MAPSQUARE_SIZE);
}

void maptpl::draw_base_tile(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  selbaseimg->draw(x,y,da_opt);
}

void maptpl::draw()
{
  draw_walkables();
  draw_base_tile();
  mapselect::draw();
}

void mapobject::init()
{
  nbr_of_parts=0;
  length=height=0;
  part=NULL;
#ifdef _EDIT_
  in_editor=false;
#endif
}

void mapobject::clear()
{
  if(nbr_of_parts) delete[] part;
}

mapobject::mapobject() : maptpl(0,0,1,1,9,9)
{
  init();
#ifdef _DEBUG_
  cout << "mapobject() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

mapobject::~mapobject()
{
  clear();
#ifdef _DEBUG_
  cout << "~mapobject() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

void mapobject::play()
{
  u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    part[i].play();
}

void mapobject::stop()
{
  u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    part[i].stop();
}

void mapobject::rewind()
{
  u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    part[i].rewind();
}

mapobject &mapobject::operator =(const mapobject &mo)
{
  u_int16 i,j;
  posx=mo.posx;
  posy=mo.posy;
  length=mo.length;
  height=mo.height;
  basex=mo.basex;
  basey=mo.basey;
  nbr_of_parts=mo.nbr_of_parts;
  currentpart=mo.currentpart;
  for(i=0;i<maptpl::length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  maptpl::length=mo.maptpl::length;
  maptpl::height=mo.maptpl::height;
  placetpl=new (mapsquaretpl*)[maptpl::length];
  for(i=0;i<maptpl::length;i++)
   { 
     placetpl[i]=new mapsquaretpl[maptpl::height];
     for(j=0;j<maptpl::height;j++)
       placetpl[i][j].walkable=mo.placetpl[i][j].walkable;
   }
  delete[] part;
  part=new animation_off[nbr_of_parts];
  for(i=0;i<nbr_of_parts;i++)
    part[i]=mo.part[i];
  return *this;
}

void mapobject::update()
{
  static u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    part[i].update();
}

void mapobject::draw_free(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  static u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    part[i].draw(x,y,da_opt);
}

void mapobject::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  draw_free(x-basex*MAPSQUARE_SIZE,y-basey*MAPSQUARE_SIZE,da_opt);
}

void mapobject::draw_border_free(u_int16 x, u_int16 y, 
				 drawing_area * da_opt=NULL)
{
  screen::drawbox(x,y,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x,y+height,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x+length,y,1,height+1,0xFFFFFF,da_opt);
  screen::drawbox(x,y,1,height+1,0xFFFFFF,da_opt);
}

void mapobject::draw_border(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  draw_border_free(x-basex*MAPSQUARE_SIZE,y-basey*MAPSQUARE_SIZE,da_opt);
}

s_int8 mapobject::get(gzFile file)
{
  u_int16 i;
  if(part) delete[] part; 
  gzread(file,&nbr_of_parts,sizeof(nbr_of_parts));
  currentpart=0;
  part=new animation_off[nbr_of_parts];
  for(i=0;i<nbr_of_parts;i++)
    {
      part[i].get(file);
      part[i].play();
    }
  maptpl::get(file);
  resize_view(9,9);
  calculate_dimensions();
  return 0;
}

s_int8 mapobject::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return -1;
  retvalue=get(file);
  gzclose(file);
  return retvalue;
}

u_int16 mapobject::get_length()
{
  return length;
}

u_int16 mapobject::get_height()
{
  return height;
}

#ifdef _EDIT_
s_int8 mapobject::put(gzFile file)
{
  u_int16 i;
  gzwrite(file,&nbr_of_parts,sizeof(nbr_of_parts));
  for(i=0;i<nbr_of_parts;i++)
    part[i].put(file);
  maptpl::put(file);
  return 0;
}

s_int8 mapobject::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return -1;
  retvalue=put(file);
  gzclose(file);
  return retvalue;
}
#endif


#ifdef _EDIT_

void mapobject::zoom(u_int16 sx, u_int16 sy, mapobject * src)
{
  u_int16 i;
  clear();
  init();
  nbr_of_parts=src->nbr_of_parts;
  part=new animation_off[nbr_of_parts];
  for(i=0;i<nbr_of_parts;i++)
    {
      part[i].zoom((src->part[i].length*sx)/src->length,
		   (src->part[i].height*sy)/src->height,&src->part[i]);
      part[i].xoffset=(src->part[i].xoffset*sx)/src->length;
      part[i].yoffset=(src->part[i].yoffset*sy)/src->height;
      part[i].play();
    }
}

void mapobject::zoom_to_fit(u_int16 sm, mapobject * src)
{
  u_int16 nl,nh,m;
  m=src->get_length()>=src->get_height()?src->get_length():src->get_height();
  if(m<sm)
    {
      nl=src->get_length();
      nh=src->get_height();
    }
  else
    {
      if(m==src->get_length())
	{
	  nl=sm;
	  nh=(src->get_height()*sm)/src->get_length();
	}
      else
	{
	  nh=sm;
	  nl=(src->get_length()*sm)/src->get_height();
	}
    }
  zoom(nl,nh,src);
}

void mapobject::init_parts()
{
  u_int16 i;
  for(i=0;i<nbr_of_parts;i++)
    {
      part[i].rewind();
      part[i].play();
    }
}

u_int16 mapobject::increase_part(u_int16 c)
{
  c++;
  if(c==nbr_of_parts) c=0;
  return c;
}

u_int16 mapobject::decrease_part(u_int16 c)
{
  if(c==0) c=nbr_of_parts-1;
  else c--;
  return c;
}

void mapobject::set_currentpart(u_int16 p)
{
  currentpart=p;
  must_upt_label_part=true;
}

void mapobject::set_part_xoffset(u_int16 p, s_int16 xoff)
{
  if(xoff<0) return;
  part[p].xoffset=xoff;
  must_upt_label_part=true;
}

void mapobject::set_part_yoffset(u_int16 p, s_int16 yoff)
{
  if(yoff<0) return;
  part[p].yoffset=yoff;
  must_upt_label_part=true;
}

void mapobject::update_label_part()
{
  if(nbr_of_parts)
    {
      sprintf(label_txt,"Current part: %d/%d\nOffset: %d, %d\n"
	      "Obj. size: %d, %d\n%s",
	      currentpart,nbr_of_parts,part[currentpart].xoffset, 
	      part[currentpart].yoffset,length,height,
	      part[currentpart].is_empty()?"Part is empty\n":"");
      if(show_grid)
	{
	  static char tmps[300];
	  sprintf(tmps,"\nCursor:\nX: %d\nY: %d\n"
		  "\nObject size:\nLength: %d\nHeight: %d",
		  posx,posy,length,height);
	  strcat(label_txt,tmps);
	}
    }
  else
    sprintf(label_txt,"No parts yet!");
  label_part->set_text(label_txt);
  must_upt_label_part=false;
}

void mapobject::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save mapobject as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapobject::update_editor),
			     makeFunctor(*this,
					 &mapobject::draw_editor));
  if(!s) return;
  if(save(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error saving!");
      wi->wait_for_keypress(makeFunctor(*this,&mapobject::update_editor),
			    makeFunctor(*this,&mapobject::draw_editor));
      delete wi;
    }
  delete qw;
}

void mapobject::load()
{
  mapobject * t=new mapobject;
  win_query * qw=new win_query(70,40,th,font,"Load mapobject:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapobject::update_editor),
			     makeFunctor(*this,
					 &mapobject::draw_editor));
  if(!s)
    {
      delete t;
      delete qw;
      return;
    }
  if(t->load(s)) 
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&mapobject::update_editor),
			    makeFunctor(*this,&mapobject::draw_editor));
      delete wi;
    }
  else 
    {
      currentpart=0;
      *(mapobject*)this=*t;
    }
  delete t;
  delete qw;
  if(in_editor) 
    {
      must_upt_label_part=true;
    }
}

void mapobject::add_animation()
{
  animation anim;
  u_int16 p;
  win_query * qw=new win_query(70,40,th,font,"Load animation: (Type \"new\" for editing a new one)");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapobject::update_editor),
			     makeFunctor(*this,
					 &mapobject::draw_editor));
  if(!s) return;
  if(strcmp(s,"new"))
    if(anim.load(s))
      {
	win_info * wi=new win_info(70,40,th,font,"Error loading animation!");
	wi->wait_for_keypress(makeFunctor(*this,&mapobject::update_editor),
			      makeFunctor(*this,&mapobject::draw_editor));
	delete wi;
	delete qw;
	return;
      }
  do
    {
      char tmp[255];
      char * s2;
      sprintf(tmp,"Insert at pos(0-%d): (Default %d)",nbr_of_parts,
	      nbr_of_parts);
      win_query * qw2=new win_query(70,40,th,font,tmp);
      s2=qw2->wait_for_text(makeFunctor(*this,
				       &mapobject::update_editor),
			   makeFunctor(*this,
				       &mapobject::draw_editor));
      if(!s2)
	{ 
	  delete qw2;
	  delete qw;
	  return;
	}
      if(!s2[0]) p=nbr_of_parts;
      else p=atoi(s2);
      delete qw2;
    }
  while(p>nbr_of_parts);
  insert_animation(anim,p);
  if(!strcmp(s,"new")) part[p].editor();
  delete qw;
  init_parts();
  must_upt_label_part=true;
}

s_int8 mapobject::insert_animation(animation &an, u_int16 pos)
{
  animation_off * oldpart=part;
  u_int16 i;
  if(pos>nbr_of_parts) return -2;
  part=new animation_off[++nbr_of_parts];
  for(i=0;i<pos;i++)
    part[i]=oldpart[i];
  part[pos]=an;
  part[pos].play();
  for(i=pos+1;i<nbr_of_parts;i++)
    part[i]=oldpart[i-1];
  delete[] oldpart;
#ifdef _DEBUG_
  cout << "Added animation: " << nbr_of_parts << " total in mapobject.\n";
#endif
  return 0;
}

s_int8 mapobject::delete_animation(u_int16 pos)
{
  animation_off * oldpart=part;
  u_int16 i;
  if(pos>nbr_of_parts-1) return -2;
  part=new animation_off[--nbr_of_parts];
  for(i=0;i<pos;i++)
    part[i]=oldpart[i];
  for(i=pos;i<nbr_of_parts;i++)
    part[i]=oldpart[i+1];
  delete[] oldpart;
  if(currentpart>=nbr_of_parts) currentpart=nbr_of_parts-1;
  if(in_editor) 
    {
      must_upt_label_part=true;
    }
#ifdef _DEBUG_
  cout << "Removed part: " << nbr_of_parts << " total in mapobject.\n";
#endif
  if(!nbr_of_parts) 
    {
      part=NULL;
      currentpart=0;
    }
  return 0;
}

void mapobject::set_part_offset(u_int16 partnbr, u_int16 x, u_int16 y)
{
  part[partnbr].set_offset(x,y);
}

void mapobject::resize_grid()
{
  u_int16 mx=0,my=0;
  u_int16 rx,ry;
  u_int16 i;
  if(!nbr_of_parts) return;
  for(i=0;i<nbr_of_parts;i++)
    {
      if(mx<part[i].length+part[i].xoffset) mx=part[i].length+part[i].xoffset;
      if(my<part[i].height+part[i].yoffset) my=part[i].height+part[i].yoffset;
    }
  rx=mx/20+(mx%20!=0);
  ry=my/20+(my%20!=0);
  if(rx==maptpl::length && ry==maptpl::height) return;
  resize(rx,ry);
  resize_view(9,9);
}

inline void mapobject::calculate_dimensions()
{
  u_int16 i;
  length=0;
  height=0;
  for(i=0;i<nbr_of_parts;i++)
    {
      u_int16 tl,th;
      if((tl=part[i].get_length()+part[i].xoffset)>length)
	length=tl;
      
      if((th=part[i].get_height()+part[i].yoffset)>height)
	height=th;
    }
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

enum dir{left, right, up, down};

inline void move_part(u_int16 p)
{
 
}

void mapobject::update_editor_keys()
{

  if(input::has_been_pushed(SDLK_g))
    {show_grid=show_grid==true?false:true; must_upt_label_part=true;}

  if(input::has_been_pushed(SDLK_F5))
    { save(); }
  if(input::has_been_pushed(SDLK_F6))
    { load(); }
  if(input::has_been_pushed(SDLK_a))
    { add_animation(); }
  if(input::has_been_pushed(SDLK_d))
    { delete_animation(currentpart); }

  if(!nbr_of_parts) return;

  if(testkey(SDLK_KP_PLUS))
    if(SDL_GetModState()&KMOD_LSHIFT)
      {
	if(currentpart<nbr_of_parts-1)
	  {
	    animation_off tmp;
	    tmp=part[currentpart];
	    part[currentpart]=part[currentpart+1];
	    part[currentpart+1]=tmp;
	    currentpart++;
	    must_upt_label_part=true;
	  }
      }
    else set_currentpart(increase_part(currentpart));
  
  if(testkey(SDLK_KP_MINUS))
    if(SDL_GetModState()&KMOD_LSHIFT)
      {
	if(currentpart>0)
	  {
	    animation_off tmp;
	    tmp=part[currentpart];
	    part[currentpart]=part[currentpart-1];
	    part[currentpart-1]=tmp;
	    currentpart--;
	    must_upt_label_part=true;
	  }
      }
    else set_currentpart(decrease_part(currentpart));
  
  if(input::has_been_pushed(SDLK_RETURN))
    {
      part[currentpart].editor();
      init_parts();
    }

  if(testkey(SDLK_LEFT))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	{ set_part_xoffset(currentpart,part[currentpart].xoffset-1); }
      else if (show_grid)
	{ move_cursor_left(); must_upt_label_part=true;}
    }
  if(testkey(SDLK_RIGHT))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	{ set_part_xoffset(currentpart,part[currentpart].xoffset+1); }
      else if (show_grid)
	{ move_cursor_right(); must_upt_label_part=true;}
    }
  if(testkey(SDLK_UP))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	{ set_part_yoffset(currentpart,part[currentpart].yoffset-1); }
      else if (show_grid)
	{ move_cursor_up(); must_upt_label_part=true;}
    }
  if(testkey(SDLK_DOWN))
    {
      if(SDL_GetModState()&KMOD_LSHIFT)
	{ set_part_yoffset(currentpart,part[currentpart].yoffset+1); }
      else if (show_grid)
	{ move_cursor_down(); must_upt_label_part=true;}
    }
  if(input::has_been_pushed(SDLK_SPACE))
    if(show_grid)
      toggle_walkable();
  if(input::has_been_pushed(SDLK_b))
    if(show_grid)
      set_base_tile(posx,posy);
}

void mapobject::update_editor()
{
  update();
  maptpl::update();
}

void mapobject::draw_editor()
{
  bg->draw(0,0);
  if(nbr_of_parts)
    {
      draw_free(0-(MAPSQUARE_SIZE*d_posx),0-(MAPSQUARE_SIZE*d_posy),da);
      if(!part[currentpart].is_empty())
	part[currentpart].draw_border(0-(MAPSQUARE_SIZE*d_posx),
				      0-(MAPSQUARE_SIZE*d_posy),da);
    }
  calculate_dimensions();
  if(must_upt_label_part) update_label_part();
  if(show_grid) maptpl::draw();
  container->draw();
}

void mapobject::update_and_draw()
{
  static u_int16 i;
  for(i=0;i<screen::frames_to_do;i++) update_editor();
  draw_editor();
}

void mapobject::editor()
{
  image temp;
  string t;
  t=WIN_DIRECTORY;
  t+=WIN_BACKGROUND_DIRECTORY;
  t+=WIN_THEME_ORIGINAL;
  t+=WIN_BACKGROUND_FILE;
  temp.load_pnm(t.data());
  bg=new image(320,240);
  bg->putbox_tile_img(&temp);
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);
  container=new win_container(200,12,110,216,th);
  label_status=new win_label(5,5,100,30,th,font);
  label_part=new win_label(5,70,100,120,th,font);
  container->add(label_status);
  container->add(label_part);
  container->set_border_visible(true);
  container->set_visible_all(true);
  currentpart=0;
  must_upt_label_part=true;
  show_grid=false;
  in_editor=true;
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      static u_int16 i;
      input::update();
      resize_grid();
      for(i=0;i<screen::frames_to_do;i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }
  in_editor=false;
  delete container;
  delete bg;
  delete th;
  delete font;
}
#endif
