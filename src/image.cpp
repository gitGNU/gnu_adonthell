// $Id$
/*
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "types.h"
#include "image.h"

SDL_Rect image::sr;
SDL_Rect image::dr;
#ifdef DEBUG
u_int16 image::a_d_diff=0;
#endif

drawing_area::drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph)
{
  x=px;
  y=py;
  w=pw;
  h=ph;
  draw_to=NULL;
}

drawing_area::drawing_area()
{
  x=0;
  y=0;
  w=0;
  h=0;
  draw_to=NULL;
}

bool drawing_area::is_x_in(s_int16 px)
{
  return((px-x>0)&&(px-(x+w)<0));
}

bool drawing_area::is_y_in(s_int16 py)
{
  return((py-y>0)&&(py-(y+h)<0));
}

bool drawing_area::is_point_in(s_int16 px, s_int16 py)
{
  return(is_x_in(px) && is_y_in(py));
}

void drawing_area::assign_drawing_area(drawing_area * da)
{
  draw_to=da;
}

void drawing_area::detach_drawing_area()
{
  draw_to=NULL;
}

SDL_Rect drawing_area::get_rects()
{
  if(draw_to)
    {
      SDL_Rect ret;
      SDL_Rect temp=draw_to->get_rects();
      ret.x=temp.x>x?temp.x:x;
      ret.y=temp.y>y?temp.y:y;
      ret.w=(temp.x+temp.w)<(x+w)?
	temp.x+temp.w-ret.x>0?temp.x+temp.w-ret.x:0:
	x+w-ret.x>0?x+w-ret.x:0;
      ret.h=(temp.y+temp.h)<(y+h)?
	temp.y+temp.h-ret.y>0?temp.y+temp.h-ret.y:0:
	y+h-ret.y>0?y+h-ret.y:0;
      return ret;
    }
  else return *this;
}

drawing_area &drawing_area::operator = (SDL_Rect & r)
{
  x=r.x;
  y=r.y;
  w=r.w;
  h=r.h;
  return *this;
}

image &image::operator =(image &im)
{
  length=im.length;
  height=im.height;
  draw_to=im.draw_to;
  bytes_per_pixel=im.bytes_per_pixel;
  mask_on=im.mask_on;
  alpha=im.alpha;
  if(data) SDL_FreeSurface(data);
#ifdef _EDIT
  if(simpledata) free(simpledata);
#endif
  data=SDL_ConvertSurface(im.data,im.data->format,im.data->flags);
#ifdef _EDIT
  simpledata=(void *)calloc(length*height,3);
  memcpy(simpledata,im.simpledata,length*height*3);
#endif
  return *this;
}

void image::init()
{
  bytes_per_pixel=0;
  data = NULL;
  length=height=0;
  mask_on=false;
  alpha=0;
  draw_to=NULL;
#ifdef _EDIT
  simpledata=NULL;
#endif
}

image::image ()
{
#ifdef DEBUG
  cout << "image() called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
  init();
}

image::image (u_int16 l, u_int16 h)
{
#ifdef DEBUG
  cout << "image(u_int16, u_int16) called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
  length=l;
  height=h;
  bytes_per_pixel=screen::bytes_per_pixel;
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,length, height,
			    bytes_per_pixel*8,0,0,0,0);
#ifdef _EDIT
  simpledata=(void*)calloc(length*height,3);
#endif
  mask_on=false;
  alpha=0;
  draw_to=NULL;
}

image::~image()
{
  clear();
#ifdef DEBUG
  cout << "~image() called, "<< --a_d_diff
       << " objects currently allocated\n";
#endif
}

void image::clear()
{
  SDL_FreeSurface(data);
  data=NULL;
#ifdef _EDIT
  free(simpledata);
#endif
}

void image::resize(u_int16 l, u_int16 h)
{
  if ((length==l)&&(height==h)) return;
  if (data) SDL_FreeSurface(data);
  bytes_per_pixel=screen::bytes_per_pixel;
  length=l;
  height=h;
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,length, height,
			    bytes_per_pixel*8,0,0,0,0);
#ifdef _EDIT
  if(simpledata) free(simpledata);
  simpledata=(void *)calloc(length*height,3);
#endif  
}

void image::assign_drawing_area(drawing_area * da)
{
  draw_to=da;
}

void image::detach_drawing_area()
{
  draw_to=NULL;
}

void image::get_rects(s_int16 x, s_int16 y)
{
  if(draw_to)
    {
      drawing_area da_int, im_zone(x,y,length, height);
      SDL_Rect t=draw_to->get_rects();
      da_int=t;
      im_zone.assign_drawing_area(&da_int);
      dr=im_zone.get_rects();
      sr=dr;
      sr.x=x<dr.x?dr.x-x:0;
      sr.y=y<dr.y?dr.y-y:0;
    }
  else
    {
      sr.x=0;
      sr.y=0;
      sr.w=length;
      sr.h=height;

      dr.x=x;
      dr.y=y;
      dr.w=length;
      dr.h=height;
    }  
}

bool image::get_mask()
{
  return(mask_on);
}

void image::set_mask(bool m)
{
  if((m)&&(!mask_on))
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::trans);
      mask_on=true;
    }
  else if((!m)&&(mask_on))
    { 
      SDL_SetColorKey(data,0,0); 
      mask_on=false;    
    }
}

u_int8 image::get_trans()
{
  return(alpha);
}    

void image::set_trans(u_int8 t)
{
  if((!t)&&(alpha)) SDL_SetAlpha(data,0,0);
  alpha=t;
}

void image::draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL)
{
  drawing_area * oldda=NULL;
  if(data==NULL) return;

  if(da_opt)
    {
      oldda=draw_to;
      draw_to=da_opt;
    }

  get_rects(x,y);
  if(!dr.w||!dr.h) return;
  if(alpha) SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
  if(da_opt) draw_to=oldda;  
}

void image::putbox (s_int16 x, s_int16 y, drawing_area * da_opt)
{  
  if(data==NULL) return;
  set_mask(false);
  set_trans(0);
  
  draw(x,y,da_opt);
}

void image::putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(true);
  set_trans(0);

  draw(x,y,da_opt);
}

void image::putbox_trans (s_int16 x, s_int16 y, u_int8 a, 
			  drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(false);
  set_trans(a);

  draw(x,y,da_opt);
}

void image::putbox_mask_trans (s_int16 x, s_int16 y, u_int8 a, 
			       drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(true);
  set_trans(a);

  draw(x,y,da_opt);
}

void image::putbox_tile_img(image * source)
{
  u_int16 posx;
  u_int16 posy;
  for(posy=0; posy<height; posy+=source->height)
    for(posx=0; posx<length; posx+=source->length)
      putbox_img(source,posx,posy);
}

void image::putbox_img(image * source, u_int16 x, u_int16 y)
{
  sr.x=0;
  sr.y=0;
  sr.w=source->length;
  sr.h=source->height;
  dr.x=x;
  dr.y=y;
  dr.w=source->length;
  dr.h=source->height;
  SDL_BlitSurface(source->data, &sr, data, &dr);
}


void image::putbox_mask_img (image * source, u_int16 x, u_int16 y)
{
  sr.x=0;
  sr.y=0;
  sr.w=source->length;
  sr.h=source->height;
  dr.x=x;
  dr.y=y;
  dr.w=source->length;
  dr.h=source->height;
  if(!mask_on) 
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::trans);
      mask_on=true;
    }
  SDL_BlitSurface(source->data, &sr, data, &dr);
}

void image::putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			     u_int16 bh,u_int16 xo, u_int16 yo)
{
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  SDL_BlitSurface(source->data, &sr, data, &dr);
}

void image::get_from_screen(s_int16 x, s_int16 y)
{
  sr.x=x;
  sr.y=y;
  sr.w=length;
  sr.h=height;
  dr.x=0;
  dr.y=0;
  dr.w=length;
  dr.h=height;
  SDL_BlitSurface(screen::vis,&sr,data,&dr);
}

s_int8 image::get(char * file)
{
  SDL_Surface * tmp;

  tmp=IMG_Load(file);
  length=tmp->w;
  height=tmp->h;
  
  data=SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
  if (!data) return(-1);
  bytes_per_pixel=data->format->BytesPerPixel;
  return(0);
}


s_int8 image::get(SDL_RWops * file)
{
  s_int8 ret;
  SDL_RWread(file,&mask_on,sizeof(mask_on),1);
  SDL_RWread(file,&alpha,sizeof(alpha),1);   
  ret=get_raw(file);
  if(!ret)
    if(mask_on)
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::trans);
  return ret;
}

s_int8 image::load(char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(1);
  get(file);
  SDL_RWclose(file);
  return(0);
}

s_int8 image::get_raw(SDL_RWops * file)
{
#ifndef _EDIT
  void * simpledata;
#endif
  SDL_Surface * tmp2;
  simpledata=read_pnm(file,&length,&height);

  tmp2=SDL_CreateRGBSurfaceFrom(simpledata,length,height,24,
				length*3,0x0000FF,0x00FF00,0xFF0000,0);
  data=SDL_DisplayFormat(tmp2);
  bytes_per_pixel=screen::bytes_per_pixel;
  SDL_FreeSurface(tmp2);

#ifndef _EDIT
  free(simpledata);
#endif
  if (!data) return(-1);
  return(0);
}

s_int8 image::load_raw(char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(1);
  get_raw(file);
  SDL_RWclose(file);
  return(0);
}

s_int8 image::get_pnm(SDL_RWops * file)
{
  return(get_raw(file));
}

s_int8 image::load_pnm(char * fname)
{
  return(load_raw(fname));
}

#ifdef _EDIT
s_int8 image::put(SDL_RWops * file)
{
  SDL_RWwrite(file,&mask_on,sizeof(mask_on),1);
  SDL_RWwrite(file,&alpha,sizeof(alpha),1);
  return(put_raw(file));
}

s_int8 image::save(char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(1);
  put(file);
  SDL_RWclose(file);
  return(0);
}

s_int8 image::put_raw(SDL_RWops * file)
{
  pnmput(file,simpledata,length,height);
  return(0);
}

s_int8 image::save_raw(char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(1);
  put_raw(file);
  SDL_RWclose(file);
  return(0);
}

s_int8 image::put_pnm(SDL_RWops * file)
{
  return(put_raw(file));
}

s_int8 image::save_pnm(char * fname)
{
  return(save_raw(fname));
}

#endif

inline u_int32 image::get_pix(u_int16 x, u_int16 y)
{
  u_int32 offset=((y*length)+x)*bytes_per_pixel;
  u_int32 retvalue=0;
  memcpy(&retvalue,(char*)data->pixels+offset,bytes_per_pixel);
  return(retvalue);
}

inline void image::put_pix(u_int16 x, u_int16 y, u_int32 col)
{
  u_int32 offset=((y*length)+x)*bytes_per_pixel;
  memcpy((char*)data->pixels+offset,&col,bytes_per_pixel);
}

void image::zoom(image * src)
{
  u_int16 i,j;
  u_int32 temp;
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      {
	temp=src->get_pix(i*src->length/length,j*src->height/height);
	put_pix(i,j,temp);
      }
}

void image::brightness(image * src, u_int16 cont)
{
  u_int16 i,j;
  u_int8 ir, ig, ib;
  u_int32 temp=0;
  
  resize(src->length,src->height);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      {
	temp=src->get_pix(i,j);
	SDL_GetRGB(temp,src->data->format,&ir,&ig,&ib);
	ir=(ir*cont)>>8;
	ig=(ig*cont)>>8;
	ib=(ib*cont)>>8;
	temp=SDL_MapRGB(data->format,ir,ig,ib);
	put_pix(i,j,temp);
      }
}
