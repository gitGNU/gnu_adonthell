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
#include <string>
#include <vector>
#include <list>
#include "mapview.h"

#ifdef _EDIT_
#define OBJSMPLSIZE 40
#endif 

class mapsquare_tile
{
  u_int16 objnbr;
  bool is_base;
  list<mapsquare_tile>::iterator base_tile;
  u_int16 x,y;

 public:
  mapsquare_tile();
  ~mapsquare_tile();
  
  bool operator < (const mapsquare_tile & mt);
  bool operator <= (const mapsquare_tile & mt);
  
  void draw(mapview * mv);
  void draw_border(mapview * mv);
  void draw_base_tile(mapview * mv);
  friend class mapsquare;
  friend class landsubmap;
  friend class landmap;
  friend class mapview;
};

class mapsquare
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#else ifdef _EDIT_
  static u_int16 a_d_diff;
#endif
  
  u_int16 type; // Terrain type ; need to be defined later

  list<mapsquare_tile> tiles;
  list<mapsquare_tile>::iterator base_begin;

 public:
  mapsquare();
  ~mapsquare();

  // Draw takes only a mapview as argument.
  void draw(mapview * mv);
  void draw(s_int16 x, s_int16 y, mapobject * pattern, 
	    drawing_area * da_opt=NULL);
  friend class mapsquare_tile;
  friend class landsubmap;
  friend class landmap;
  friend class mapview;
};  // mapsquare

class landmap;

class landsubmap
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#else ifdef _EDIT_
  static u_int16 a_d_diff;
#endif
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
#endif

  u_int16 get_length();
  u_int16 get_height();

  s_int8 set_square_pattern(u_int16 px, u_int16 py, 
			    u_int16 patnbr);

  void draw_square(u_int16 x, u_int16 y, u_int16 px, u_int16 py, 
		   mapobject * pattern, drawing_area * da_opt=NULL);
  //  void draw(mapobject * pattern, drawing_area * da_opt=NULL);
  friend class mapview;
};  // landsubmap


class landmap
{
#ifdef _DEBUG_
  static u_int16 a_d_diff;
#else ifdef _EDIT_
  static u_int16 a_d_diff;
#endif

#ifdef _EDIT_
  mapobject * mini_pattern;
  
  void update_current_tile();

  void increase_obj_here();
  void decrease_obj_here();
  void increase_currentobj();
  void decrease_currentobj();

  void reset_objs();

#endif


 public:
  mapobject * pattern;
  u_int16 nbr_of_patterns;

  vector<string> objsrc;

  landsubmap ** submap;
  u_int16 nbr_of_submaps;

 public:
  landmap();
  void clear();
  ~landmap();

  landmap& operator =(const landmap& lm);

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif

  // Adds a submap to the map. Returns 0 if ok (in this case, the submap
  // number is nbr_of_submaps-1)
  s_int8 add_submap();
  s_int8 add_submap(u_int16 l, u_int16 h);

  // Remove the submap nbr from the submaps list. Returns 0 if ok, otherwise
  // an error code.
  s_int8 remove_submap(u_int16 nbr);

  s_int8 set_square_pattern(u_int16 smap, u_int16 px, u_int16 py, 
			    u_int16 patnbr);

  void remove_obj_from_square(u_int16 smap,
			      list<mapsquare_tile>::iterator obj);  
  void update();

  s_int8 insert_mapobject(mapobject &an, u_int16 pos,
			  const char * srcfile="");
  s_int8 delete_mapobject(u_int16 pos);

  void draw_square(u_int16 smap, u_int16 x, u_int16 y, u_int16 px, u_int16 py,
		   drawing_area * da_opt=NULL);
  friend class mapview;
};  // landmap

#endif
