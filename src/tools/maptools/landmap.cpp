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

#include "landmap.h"

#ifdef _DEBUG_
u_int16 mapsquare::a_d_diff=0;
u_int16 landsubmap::a_d_diff=0;
u_int16 landmap::a_d_diff=0;
#else ifdef _EDIT_
u_int16 mapsquare::a_d_diff=0;
u_int16 landsubmap::a_d_diff=0;
u_int16 landmap::a_d_diff=0;
#endif

mapsquare_tile::mapsquare_tile()
{
  objnbr=0;
  is_base=false;
}

mapsquare_tile::~mapsquare_tile()
{

}

bool mapsquare_tile::operator < (const mapsquare_tile & m)
{
  return (m.y>y || (m.y==y && m.x>x));
}

bool mapsquare_tile::operator <= (const mapsquare_tile & m)
{
  return (m.y>y || (m.y==y && m.x>=x));
}

void mapsquare_tile::draw(mapview * mv)
{
  if(is_base)
    {
      u_int16 rx, ry;
      rx=(mv->posx>mv->ctrx)?x-(mv->posx-mv->ctrx):x;
      ry=(mv->posy>mv->ctry)?y-(mv->posy-mv->ctry):y;
      mv->m_map->pattern[objnbr].draw
	(rx*MAPSQUARE_SIZE-mv->offx,ry*MAPSQUARE_SIZE-mv->offy,mv->da);
    }
  else
    base_tile->draw(mv);
}

void mapsquare_tile::draw_border(mapview * mv)
{
  if(is_base)
    {
      u_int16 rx, ry;
      rx=(mv->posx>mv->ctrx)?x-(mv->posx-mv->ctrx):x;
      ry=(mv->posy>mv->ctry)?y-(mv->posy-mv->ctry):y;
      mv->m_map->pattern[objnbr].draw_border
	(rx*MAPSQUARE_SIZE,ry*MAPSQUARE_SIZE,mv->da);
    }
  else 
      base_tile->draw_border(mv);
}

void mapsquare_tile::draw_base_tile(mapview * mv)
{
  if(is_base)
    {
      u_int16 rx,ry;
      rx=(mv->posx>mv->ctrx)?x-(mv->posx-mv->ctrx):x;
      ry=(mv->posy>mv->ctry)?y-(mv->posy-mv->ctry):y;
      mv->m_map->pattern[objnbr].draw_base_tile
	(rx*MAPSQUARE_SIZE,ry*MAPSQUARE_SIZE,mv->da);
    }
  else
    base_tile->draw_base_tile(mv);
}

mapsquare::mapsquare()
{
  type=0;
  base_begin=tiles.end();
#ifdef _DEBUG_
  cout << "mapsquare() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

mapsquare::~mapsquare()
{
#ifdef _DEBUG_
  cout << "~mapsquare() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

void mapsquare::draw(mapview * mv)
{
  list<mapsquare_tile>::iterator i;
  i=tiles.begin();
  while(i!=tiles.end())
    {
      if(i->is_base) mv->m_map->pattern[i->objnbr].
		       draw((i->x-(mv->posx-mv->ctrx))*MAPSQUARE_SIZE,
			    (i->y-(mv->posy-mv->ctry))*MAPSQUARE_SIZE,
			    mv->da);
      else
	{
	  // Right (bottom) overflow?
	  // FIXME: not correct. For top/bottom+left/right tests, must
	  // check FIRST if the square is on a screen corner.
	  // Optimize this!

	  // Bottom Right corner test
	  if(i->x-(mv->posx-mv->ctrx)+1==mv->d_length && 
	     i->y-(mv->posy-mv->ctry)+1==mv->d_height &&
	     i->base_tile->x>i->x && i->base_tile->y>i->y)
	  i->base_tile->draw(mv);

	  // Bottom Left corner test
	  else if(i->x-(mv->posx-mv->ctrx)==0 && 
	     i->y-(mv->posy-mv->ctry)+1==mv->d_height &&
	     i->base_tile->x<i->x && i->base_tile->y>i->y)
	      mv->m_map->submap[mv->currentsubmap].land[i->x][i->base_tile->y].
		draw(mv);

	    //	    i->base_tile->draw(mv);

	  // Top Right corner test
	  else if(i->x-(mv->posx-mv->ctrx)+1==mv->d_length && 
	     i->y-(mv->posy-mv->ctry)==0 &&
	     i->base_tile->x>i->x && i->base_tile->y<i->y)
	    i->base_tile->draw(mv);

	  // Top Left corner test
	  else if(i->x-(mv->posx-mv->ctrx)==0 && 
	     i->y-(mv->posy-mv->ctry)==0 &&
	     i->base_tile->x<i->x && i->base_tile->y<i->y)
	    i->base_tile->draw(mv);

	  else if(i->x-(mv->posx-mv->ctrx)+1==mv->d_length && 
		  i->base_tile->x>i->x && i->base_tile->y>=i->y)
	    {
	      i->base_tile->draw(mv);
	      //	      break;
	    }
	  // Right overflow?
	  else if(i->x-(mv->posx-mv->ctrx)+1==mv->d_length && 
		  i->base_tile->x>i->x && i->base_tile->y==i->y)
	    {
	      i->base_tile->draw(mv);
	      //	      break;
	    }
	  // Left overflow?
	  else if(i->x-(mv->posx-mv->ctrx)==0 && i->base_tile->x<i->x 
		  && i->base_tile->y==i->y)
	    {
	      i->base_tile->draw(mv);
	      //	      break;
	    }
	  // Bottom overflow?
	  else if(i->y-(mv->posy-mv->ctry)+1==mv->d_height && 
		  i->base_tile->y>i->y && i->base_tile->x==i->x)
	    {
	      mv->critical_draw.push_front(*i);
	      i->base_tile->draw(mv);
	      //	      break;
	    }
	  // Top overflow?
	  else if(i->y-(mv->posy-mv->ctry)==0 && i->base_tile->y<i->y 
		  /*		  && i->base_tile->x==i->x*/)
	    {
	      if(!(i->x-i->base_tile->x+mv->m_map->pattern[i->objnbr].basex))
		{
		  i->base_tile->draw(mv);
		}
	      //	      break;
	    }
	}
      i++;
    }
}

void mapsquare::draw(s_int16 x, s_int16 y, mapobject * pattern,
		     drawing_area * da_opt=NULL)
{
  list<mapsquare_tile>::iterator i;
  i=tiles.begin();
  while(i!=tiles.end())
    {
      if(i->is_base) pattern[i->objnbr].draw(x,y,da_opt);
      i++;
    }
  /*  static u_int16 i;
  for(i=ground1;i<nbr_of_tiles;i++)
  if(active[i]) pattern[tile[i]].draw(x,y,da_opt);*/
}

landsubmap::landsubmap()
{
  length=height=0;
  land=NULL;
#ifdef _DEBUG_
  cout << "landsubmap() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

landsubmap::landsubmap(u_int16 l, u_int16 h)
{
  allocmap(l,h);
#ifdef _DEBUG_
  cout << "landsubmap() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

landsubmap::~landsubmap()
{
  destroymap();
#ifdef _DEBUG_
  cout << "~landsubmap() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

landsubmap& landsubmap::operator = (landsubmap & sm)
{
  u_int16 i,j;
  bool difsize=false;
  if(length!=sm.length || height!=sm.height)
    {
      difsize=true;
      destroymap();
    }
  memcpy(this,&sm,sizeof(landsubmap));
  if(difsize)
  allocmap(length,height);
  for(i=0;i<length;i++)
    for(j=0;j<height;j++)
      land[i][j]=sm.land[i][j];
  
  return *this;
}

inline void landsubmap::allocmap(u_int16 l, u_int16 h)
{
  u_int16 i;
  length=l;
  height=h;
  land=new (mapsquare*)[l];
  for(i=0;i<l;i++)
    land[i]=new mapsquare[h];
}

inline void landsubmap::destroymap()
{
  u_int16 i;
  for(i=0;i<length;i++)
    delete[] land[i];
  delete[] land;
  length=0;
  height=0;
}

void landsubmap::resize(u_int16 l, u_int16 h)
{
  u_int16 i,j;
  if(length==l && height==h) return;
  landsubmap tmp(length,height);
  tmp=*this;

  destroymap();
  
  allocmap(l,h);
  for(i=0;i<l;i++)
    for(j=0;j<h;j++)
      if(i<tmp.length && j<tmp.height) land[i][j]=tmp.land[i][j];
}

u_int16 landsubmap::get_length()
{
  return length;
}

u_int16 landsubmap::get_height()
{
  return height;
}

void landsubmap::draw_square(u_int16 x, u_int16 y, u_int16 px, u_int16 py,
			     mapobject * pattern, drawing_area * da_opt=NULL)
{
  land[px][py].draw(x,y,pattern,da_opt);
}

/*void landsubmap::draw(mapobject * pattern, drawing_area * da_opt=NULL)
{
  static u_int16 i,j;
  static u_int16 i0,j0;

  i0=(posx<ctrx)?0:(posx>length-(ctrx+1))?length-(2*ctrx+1):posx-ctrx;
  j0=(posy<ctry)?0:(posy>height-(ctry+1))?height-(2*ctry+1):posy-ctry;

  for(j=j0;j<j0+d_height && j<height;j++)
    for(i=i0;i<i0+d_length && i<length;i++)
      draw_square(sx+(MAPSQUARE_SIZE*(i-i0)),sy+(MAPSQUARE_SIZE*(j-j0)),
		  i,j,pattern,da_opt);
}
*/
landmap::landmap()
{
  pattern=NULL;
#ifdef _EDIT_
  mini_pattern=NULL;
#endif
  nbr_of_patterns=0;
  submap=NULL;
  nbr_of_submaps=0;
#ifdef _DEBUG_
  cout << "landmap() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
}

landmap::~landmap()
{
  if(submap) delete[] submap;
  if(pattern) delete[] pattern;
#ifdef _EDIT_
  if(mini_pattern) delete[] mini_pattern;
#endif
#ifdef _DEBUG_
  cout << "~landmap() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

s_int8 landmap::add_submap()
{
  u_int16 i;
  landsubmap * tmp=submap;
  if(nbr_of_submaps==255) return -1;
  submap=new landsubmap[++nbr_of_submaps];
  for(i=0;i<nbr_of_submaps-1;i++)
    submap[i]=tmp[i];
  if(tmp) delete[] tmp;
  return 0;
}

s_int8 landmap::add_submap(u_int16 l, u_int16 h)
{
  s_int8 err=add_submap();
  if(err) return err;
  submap[nbr_of_submaps-1].resize(l,h);
  return 0;
}

s_int8 landmap::remove_submap(u_int16 nbr)
{
  if(nbr>nbr_of_submaps) return -1;
  return 0;
}

void landmap::update()
{
  static u_int16 i;
  for(i=0;i<nbr_of_patterns;i++)
    {
      pattern[i].update();
#ifdef _EDIT_
      mini_pattern[i].update();
#endif
    }
}

s_int8 landmap::add_object(mapobject& an)
{
  u_int16 i;
  mapobject * tmp=pattern;
  pattern=new mapobject[++nbr_of_patterns];
  for(i=0;i<nbr_of_patterns-1;i++)
    pattern[i]=tmp[i];
  pattern[nbr_of_patterns-1]=an;
  if(tmp) delete[] tmp;
#ifdef _EDIT_
  tmp=mini_pattern;
  mini_pattern=new mapobject[nbr_of_patterns];
  for(i=0;i<nbr_of_patterns-1;i++)
    mini_pattern[i]=tmp[i];
  
  mini_pattern[i].zoom_to_fit(OBJSMPLSIZE,&an);
  if(tmp) delete[] tmp;
#endif
  return 0;
}

void landmap::reset_objs()
{
  u_int16 i;
  for(i=0;i<nbr_of_patterns;i++)
    {
      pattern[i].rewind();
#ifdef _EDIT_
      mini_pattern[i].rewind();
#endif
    }
}

s_int8 landmap::set_square_pattern(u_int16 smap, u_int16 px, u_int16 py, 
				   u_int16 patnbr)
{
  u_int16 i,j;
  mapsquare_tile t;
  
  // FIXME: Bad placement when overflowing the map from bottom right when the 
  // base tile isn't at right of the object.
  // Bad initialisation of i0/j0/ie/je ( must be init like remove_obj)

  u_int16 ie=pattern[patnbr].maptpl::get_length();
  u_int16 je=pattern[patnbr].maptpl::get_height();

  ie=px-pattern[patnbr].basex+ie>submap[smap].length?submap[smap].length-px:ie;
  je=py-pattern[patnbr].basey+je>submap[smap].height?submap[smap].height-py:je;

  u_int16 i0=(px-pattern[patnbr].basex)>0?0:pattern[patnbr].basex-px;
  u_int16 j0=(py-pattern[patnbr].basey)>0?0:pattern[patnbr].basey-py;
  // Steps:
  // -Find out where to start/stop placing the object (screen overflow)
  // -For each square, place it in the right order
  //  Don't forget to update the base_begin iterator to refer to the
  //  first base tile that's on the square.
  list<mapsquare_tile>::iterator it;

  if(patnbr>nbr_of_patterns) return -1;
  // First place the base tile, as others refers to it...
  t.objnbr=patnbr;
  t.is_base=true;
  t.x=px;
  t.y=py;
  for(it=submap[smap].land[px][py].tiles.begin();
      it!=submap[smap].land[px][py].tiles.end() && *(it->base_tile)<=t; it++);
  submap[smap].land[px][py].tiles.insert(it,t);
  it--;
  it->base_tile=it;
  
  // Get the base tile iterator for the others tiles
  t.base_tile=it;
  t.is_base=false;
  for(j=j0;j<je;j++)
    for(i=i0;i<ie;i++)
      {
	t.x=px-pattern[patnbr].basex+i;
	t.y=py-pattern[patnbr].basey+j;
	if(pattern[patnbr].basex!=i || pattern[patnbr].basey!=j)
	  {
	    for(it=submap[smap].land[t.x][t.y].tiles.begin();
		it!=submap[smap].land[t.x][t.y].tiles.end() &&
		  *(it->base_tile) <= *(t.base_tile);it++);
	    submap[smap].land[t.x][t.y].tiles.insert(it,t);
	  }
      }
  for(it=submap[smap].land[px][py].tiles.begin();
      it!=submap[smap].land[px][py].tiles.end() && *(it->base_tile)<*it;i++);
  submap[smap].land[px][py].base_begin=it;
  return 0;
}

void landmap::remove_obj_from_square(u_int16 smap,
				     list<mapsquare_tile>::iterator obj)
{
  u_int16 i,j;
  list<mapsquare_tile>::iterator it;
  u_int16 ie=pattern[obj->objnbr].maptpl::get_length();
  u_int16 je=pattern[obj->objnbr].maptpl::get_height();

  u_int16 i0=(obj->base_tile->x-pattern[obj->objnbr].basex)>0?0:
    pattern[obj->objnbr].basex-obj->base_tile->x;
  u_int16 j0=(obj->base_tile->y-pattern[obj->objnbr].basey)>0?0:
    pattern[obj->objnbr].basey-obj->base_tile->y;

  ie=obj->base_tile->x-pattern[obj->objnbr].basex+ie>submap[smap].length?
    submap[smap].length-obj->base_tile->x:ie;
  je=obj->base_tile->y-pattern[obj->objnbr].basey+je>submap[smap].height?
    submap[smap].height-obj->base_tile->y:je;

  ie-=i0;
  je-=j0;
  i0+=obj->base_tile->x-pattern[obj->objnbr].basex;
  j0+=obj->base_tile->y-pattern[obj->objnbr].basey;
  ie+=i0;
  je+=j0;

  for(j=j0;j<je;j++)
    for(i=i0;i<ie;i++)
      {
	it=submap[smap].land[i][j].tiles.begin();
	while(it!=submap[smap].land[i][j].tiles.end() && 
	      it->base_tile!=obj->base_tile) it++;
	if(it!=submap[smap].land[i][j].tiles.end())
	  {
	    submap[smap].land[i][j].tiles.erase(it);
	    for(it=submap[smap].land[i][j].tiles.begin();
		it!=submap[smap].land[i][j].tiles.end();it++);
	    for(it=submap[smap].land[i][j].tiles.begin();
		it!=submap[smap].land[i][j].tiles.end() && 
		  *(it->base_tile)<*it;it++);
	    submap[smap].land[i][j].base_begin=it;
	  }  
      }
}

s_int8 landmap::insert_mapobject(mapobject &an, u_int16 pos)
{
  mapobject * oldpat=pattern;
  u_int16 i;
  if(pos>nbr_of_patterns) return -2;
  pattern=new mapobject[++nbr_of_patterns];
  for(i=0;i<pos;i++)
    pattern[i]=oldpat[i];
  pattern[pos]=an;
  pattern[pos].play();
  for(i=pos+1;i<nbr_of_patterns;i++)
    pattern[i]=oldpat[i-1];
  delete[] oldpat;
#ifdef _EDIT_
  oldpat=mini_pattern;
  mini_pattern=new mapobject[nbr_of_patterns];
  for(i=0;i<nbr_of_patterns-1;i++)
    mini_pattern[i]=oldpat[i];
  
  mini_pattern[pos].zoom_to_fit(OBJSMPLSIZE,&an);
  delete[] oldpat;
#endif
#ifdef _DEBUG_
  cout << "Added mapobject: " << nbr_of_patterns << " total in landmap.\n";
#endif
  return 0;
}

s_int8 landmap::delete_mapobject(u_int16 pos)
{
  mapobject * oldpat=pattern;
  u_int16 i;
  if(pos>nbr_of_patterns-1) return -2;
  pattern=new mapobject[--nbr_of_patterns];
  for(i=0;i<pos;i++)
    pattern[i]=oldpat[i];
  for(i=pos;i<nbr_of_patterns;i++)
    pattern[i]=oldpat[i+1];
  delete[] oldpat;

#ifdef _EDIT_
  oldpat=mini_pattern;
  mini_pattern=new mapobject[nbr_of_patterns];

  for(i=0;i<pos;i++)
    mini_pattern[i]=oldpat[i];
  for(i=pos;i<nbr_of_patterns;i++)
    mini_pattern[i]=oldpat[i+1];
  delete[] oldpat;
#endif

#ifdef _DEBUG_
  cout << "Removed mapobject: " << nbr_of_patterns << " total in landmap.\n";
#endif
  if(!nbr_of_patterns) 
    {
      pattern=NULL;
    }
  return 0;
}

void landmap::draw_square(u_int16 smap, u_int16 x, u_int16 y, u_int16 px, 
			  u_int16 py, drawing_area * da_opt=NULL)
{
  submap[smap].draw_square(x,y,px,py,pattern,da_opt);
}
