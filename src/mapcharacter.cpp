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

#include "mapcharacter.h"

void mapcharacter::init()
{
  anim.resize(NBR_MOVES);
  for(u_int16 i=0; i<NBR_MOVES; i++)
    anim[i]=new animation;
  current_move=STAND_NORTH;
}

void mapcharacter::clear()
{
  for(u_int16 i=0;i<anim.size();i++)
    delete(anim[i]);
  anim.clear();
}

mapcharacter::mapcharacter()
{
  init();
}

mapcharacter::~mapcharacter()
{
  clear();
}

s_int8 mapcharacter::get(gzFile file)
{
  int i;
  for(i=0;i<NBR_MOVES;i++)
    {
      anim[i]->get(file);
    }
  return 0;
}

s_int8 mapcharacter::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return(-1);
  retvalue=get(file);
  gzclose(file);
  return 0;
}

void mapcharacter::update()
{
  anim[current_move]->update();
}

void mapcharacter::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  anim[current_move]->draw(x,y,da_opt);
}

void mapcharacter::go_north()
{
  current_move=WALK_NORTH;
}

void mapcharacter::go_south()
{
  current_move=WALK_SOUTH;
}

void mapcharacter::go_east()
{
  current_move=WALK_EAST;
}

void mapcharacter::go_west()
{
  current_move=WALK_WEST;
}

#ifdef _EDIT_
s_int8 mapcharacter::put(gzFile file)
{
  int i;
  for(i=0;i<NBR_MOVES;i++)
    {
      anim[i]->put(file);
    }
  return 0;
}

s_int8 mapcharacter::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return(-1);
  retvalue=put(file);
  gzclose(file);
  return 0;
}

// New inserting method, much faster: the animation pointed by an must
// however be kept in memory, and will be deleted by destructor.

void mapcharacter::insert_anim(animation * an, c_moves pos)
{
  delete anim[pos];
  anim[pos]=an;
}

void mapcharacter::save()
{
  win_query * qw=new win_query(70,40,th,font,"Save character as:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapcharacter::update_editor),
			     makeFunctor(*this,
					 &mapcharacter::draw_editor));
  if(!s) return;
  if(save(s))
    {
      win_info * wi=new win_info(70,40,th,font,"Error saving!");
      wi->wait_for_keypress(makeFunctor(*this,&mapcharacter::update_editor),
			    makeFunctor(*this,&mapcharacter::draw_editor));
      delete wi;
    }
  delete qw;
}

void mapcharacter::load()
{
  mapcharacter * t=new mapcharacter;
  win_query * qw=new win_query(70,40,th,font,"Load character:");
  char * s=qw->wait_for_text(makeFunctor(*this,
					 &mapcharacter::update_editor),
			     makeFunctor(*this,
					 &mapcharacter::draw_editor));
  if(!s)
    {
      delete t;
      delete qw;
      return;
    }
  if(t->load(s)) 
    {
      win_info * wi=new win_info(70,40,th,font,"Error loading!");
      wi->wait_for_keypress(makeFunctor(*this,&mapcharacter::update_editor),
			    makeFunctor(*this,&mapcharacter::draw_editor));
      delete wi;
    }
  else 
    {
      *(mapcharacter*)this=*t;
    }
  delete t;
  delete qw;
}

void mapcharacter::update_editor()
{
}

void mapcharacter::draw_editor()
{
  screen::clear();
  draw(20,20);
}

void mapcharacter::update_and_draw()
{
  draw_editor();
}

inline bool testkey(SDLKey k)
{
  if(SDL_GetModState()&KMOD_LCTRL)
    return((input::is_pushed(k)));
  else return ((input::has_been_pushed(k)));
}

void mapcharacter::update_editor_keys()
{
  if(input::has_been_pushed(SDLK_F5))
    save();

  if(input::has_been_pushed(SDLK_F6))
    load();
  
  if(input::has_been_pushed(SDLK_F1));
  if(input::has_been_pushed(SDLK_F2));
}

void mapcharacter::editor()
{
  u_int16 i;
  font=new win_font(WIN_THEME_ORIGINAL);
  th=new win_theme(WIN_THEME_ORIGINAL);  
  while(!input::has_been_pushed(SDLK_ESCAPE))
    {
      input::update();
      for(i=0;i<screen::frames_to_do();i++) update_editor_keys();
      update_and_draw();
      screen::show();
    }
  delete font;
  delete th;
}
#endif
