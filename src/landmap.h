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

#ifndef _LANDMAP_H
#define _LANDMAP_H

#include "mapobject.h"
#include "mapcharacter.h"
#include <string>
#include <vector>
#include <list>

#ifdef _EDIT_
#define OBJSMPLSIZE 40
#endif // _EDIT_

class mapview;

#ifndef SWIG

class mapsquare_tile
{
  u_int16 objnbr;
  bool is_base;
  list<mapsquare_tile>::iterator base_tile;
  u_int16 x,y;

 public:
  mapsquare_tile();
  ~mapsquare_tile();
  
  bool operator < (const mapsquare_tile & mt)
    { return (mt.y>y || (mt.y==y && mt.x>x)); }
  bool operator <= (const mapsquare_tile & mt)
    { return (mt.y>y || (mt.y==y && mt.x>=x)); }
  bool operator == (const mapsquare_tile & mt)
    {  return (mt.y==y && mt.x==x); }

  void draw(mapview * mv);
  void draw_border(mapview * mv);
  void draw_base_tile(mapview * mv);
  friend class mapsquare;
  friend class landsubmap;
  friend class landmap;
  friend class mapview;
};

class mapsquare_char
{
  mapcharacter * mchar;
  bool is_base;
  bool walkable;
  list<mapsquare_char>::iterator base_tile;
  u_int16 x,y;

 public:
  mapsquare_char();
  ~mapsquare_char();
  
  bool operator < (const mapsquare_char & mt);
  bool operator <= (const mapsquare_char & mt);
  bool operator == (const mapsquare_char & mt)
    {  return (mt.y==y && mt.x==x); }
  
  void draw(mapview * mv);
  /*  void draw_border(mapview * mv);
      void draw_base_tile(mapview * mv);*/
  friend class mapsquare;
  friend class landsubmap;
  friend class landmap;
  friend class mapview;
};

class mapsquare
{
#if defined _DEBUG_ || defined _EDIT_
  static u_int16 a_d_diff;
#endif // _DEBUG_
  
  u_int16 type; // Terrain type ; need to be defined later
  u_int8 walkable;

  list<mapsquare_tile> tiles;
  list<mapsquare_tile>::iterator base_begin;
  list<mapsquare_char> mapchars;
 public:
  mapsquare();
  ~mapsquare();

  // Draw takes only a mapview as argument.
  void draw(s_int16 x, s_int16 y, mapobject ** pattern, 
	    drawing_area * da_opt=NULL);
  bool is_free() 
    { 
      list<mapsquare_char>::iterator i;
      for(i=mapchars.begin();i!=mapchars.end();i++)
	if(!i->walkable) return false;
      return true;
    }
  bool is_walkable_left() { return (walkable & WALKABLE_LEFT); }
  bool is_walkable_right() { return (walkable & WALKABLE_RIGHT); }
  bool is_walkable_up() { return (walkable & WALKABLE_UP); }
  bool is_walkable_down() { return (walkable & WALKABLE_DOWN); }
  void set_walkable_left(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_LEFT);
      else walkable|=WALKABLE_LEFT;
    }
  void set_walkable_right(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_RIGHT);
      else walkable|=WALKABLE_RIGHT;
    }  
  void set_walkable_up(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_UP);
      else walkable|=WALKABLE_UP;
    }  
  void set_walkable_down(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_DOWN);
      else walkable|=WALKABLE_DOWN;
    }

  friend class mapsquare_tile;
  friend class landsubmap;
  friend class landmap;
  friend class mapview;
};  // mapsquare

class landmap;

class landsubmap
{
#if defined _DEBUG_ || defined _EDIT_
  static u_int16 a_d_diff;
#endif // _DEBUG_
 public:
  landmap * m_map;
  mapsquare ** land;          // The mapsquares refers to the parent map
  // patterns.
  u_int16 length, height;
  u_int16 d_length, d_height; // Number of tiles to draw on the screen. 
  // MUST BE EVEN! (Must be the same values as the parent's map)

 public:
  landsubmap();
  landsubmap(u_int16 l, u_int16 h);
  ~landsubmap();
  
  landsubmap& operator = (landsubmap & sm);
  inline void allocmap(u_int16 l, u_int16 h);
  inline void destroymap();
  void resize(u_int16 l, u_int16 h);

  s_int8 get(gzFile file);

#ifdef _EDIT_
  s_int8 put(gzFile file);
#endif // _EDIT_

  u_int16 get_length();
  u_int16 get_height();

  s_int8 set_square_pattern(u_int16 px, u_int16 py, 
			    u_int16 patnbr);

  void draw_square(u_int16 x, u_int16 y, u_int16 px, u_int16 py, 
		   mapobject ** pattern, drawing_area * da_opt=NULL);
  //  void draw(mapobject * pattern, drawing_area * da_opt=NULL);
  friend class mapview;
  friend class mapcharacter;
};  // landsubmap

#endif // SWIG

class landmap
{
#if defined _DEBUG_ || defined _EDIT_
  static u_int16 a_d_diff;
#endif // _DEBUG_

#ifndef SWIG
#ifdef _EDIT_
  mapobject ** mini_pattern;
  
  void update_current_tile();

  void increase_obj_here();
  void decrease_obj_here();
  void increase_currentobj();
  void decrease_currentobj();

  void reset_objs();

#endif // _EDIT_
#endif // SWIG

 public:
#ifndef SWIG
  vector<mapcharacter*> mapchar;
  mapobject ** pattern;
  vector<string> objsrc;
  landsubmap ** submap;
#endif // SWIG

  u_int16 nbr_of_patterns;
  u_int16 nbr_of_submaps;

 public:
  landmap();
  void clear();
  ~landmap();

#ifndef SWIG
  landmap& operator =(const landmap& lm);
#endif // SWIG

  u_int16 get_nbr_of_patterns() { return nbr_of_patterns; }
  u_int16 get_nbr_of_submaps() { return nbr_of_submaps; }

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif // _EDIT_

  void put_mapchar(mapcharacter * mchar, u_int16 smap, u_int16 px, u_int16 py);
  void remove_mapchar(mapcharacter * mchar, u_int16 smap, u_int16 px,
		      u_int16 py);
  // Adds a submap to the map. Returns 0 if ok (in this case, the submap
  // number is nbr_of_submaps-1)
  void add_mapcharacter(mapcharacter * m)
    {mapchar.push_back(m);}
  mapcharacter * get_mapchar(u_int16 nbr) { return mapchar[nbr]; }

  // Remove the submap nbr from the submaps list. Returns 0 if ok, otherwise
  // an error code.
  s_int8 remove_submap(u_int16 nbr);

  s_int8 set_square_pattern(u_int16 smap, u_int16 px, u_int16 py, 
			    u_int16 patnbr);

  void update();

  void draw_square(u_int16 smap, u_int16 x, u_int16 y, u_int16 px, u_int16 py,
		   drawing_area * da_opt=NULL);

  s_int8 add_submap(u_int16 l, u_int16 h);

#ifndef SWIG
  s_int8 add_submap();
  
  void remove_obj_from_square(u_int16 smap,
			      list<mapsquare_tile>::iterator obj);  
  s_int8 insert_mapobject(mapobject &an, u_int16 pos,
			  const char * srcfile="");
  s_int8 delete_mapobject(u_int16 pos);

  friend class mapview;
  friend class mapcharacter;
#endif // SWIG

#ifdef _EDIT_
  void editor();
#endif // _EDIT_
};  // landmap

#endif // _LANDMAP_H
