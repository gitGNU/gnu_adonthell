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
#include "types.h"
#include "screen.h"
#include "image.h"
#include "pnm.h"

SDL_Rect image::sr;
SDL_Rect image::dr;

drawing_area::drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph)
{
  x=px;
  y=py;
  length=pw;
  height=ph;
}

bool drawing_area::is_x_in(s_int16 px)
{
  return((px-x>0)&&(px-(x+length)<0));
}

bool drawing_area::is_y_in(s_int16 py)
{
  return((py-y>0)&&(py-(y+height)<0));
}

bool drawing_area::is_point_in(s_int16 px, s_int16 py)
{
  return(is_x_in(px) && is_y_in(py));
}

image::image ()
{
  data = NULL;
  length=height=0;
  mask_on=false;
  trans_on=false;
  draw_to=NULL;
}

image::image (u_int16 l, u_int16 h)
{
  length=l;
  height=h;
  bytes_per_pixel=screen::bytes_per_pixel;
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,length, height,
			    bytes_per_pixel,0,0,0,0);
  mask_on=false;
  trans_on=false;
  draw_to=NULL;
}

image::~image()
{
  clear();
}

void image::clear()
{
  SDL_FreeSurface(data);
  data=NULL;
}

void image::resize(u_int16 l, u_int16 h)
{
  if(!data)
    {  
      bytes_per_pixel=screen::bytes_per_pixel;
      length=l;
      height=h;
      data=SDL_CreateRGBSurface(SDL_SWSURFACE,length, height,
				bytes_per_pixel*8,0,0,0,0);
    }
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
      sr.x=(draw_to->is_x_in(x))?0:draw_to->x-x;
      sr.y=(draw_to->is_y_in(y))?0:draw_to->y-y;

      if((sr.x<0)||(sr.y<0)) return;
    
      sr.w=(x+length-sr.x>0)?
	(x+length>draw_to->x+draw_to->length)?
	draw_to->x+draw_to->length-x:x+length-sr.x:0;

      sr.h=(y+height-sr.y>0)?
	(y+height>draw_to->y+draw_to->height)?
	draw_to->y+draw_to->height-y:y+height-sr.y:0;

      dr.x=!sr.x?x:draw_to->x;
      dr.y=!sr.y?y:draw_to->y;
      dr.w=sr.w;
      dr.h=sr.h;
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
void image::putbox (s_int16 x, s_int16 y, drawing_area * da_opt)
{
  drawing_area * oldda=NULL;
  if(data==NULL) return;

  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  
  if(da_opt)
    {
      oldda=draw_to;
      draw_to=da_opt;
    }

  get_rects(x,y);
  if(!dr.w||!dr.h) return;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
  if(da_opt) draw_to=oldda;
}

void image::putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt)
{
  drawing_area * oldda=NULL;
  if(data==NULL) return;

  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  if(!mask_on) 
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::trans);
      mask_on=true;
    }

  if(da_opt)
    {
      oldda=draw_to;
      draw_to=da_opt;
    }

  get_rects(x,y);
  if(!dr.w||!dr.h) return;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
  if(da_opt) draw_to=oldda;
}

void image::putbox_trans (s_int16 x, s_int16 y, u_int8 alpha, 
			  drawing_area * da_opt)
{
  drawing_area * oldda=NULL;
  if(data==NULL) return;

  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;

  if(da_opt)
    {
      oldda=draw_to;
      draw_to=da_opt;
    }

  get_rects(x,y);
  if(!dr.w||!dr.h) return;

  SDL_BlitSurface(data, &sr, screen::vis, &dr);  
  if(da_opt) draw_to=oldda;
}

void image::putbox_mask_trans (s_int16 x, s_int16 y, u_int8 alpha, 
			       drawing_area * da_opt)
{
  drawing_area * oldda=NULL;
  if(data==NULL) return;
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::trans);
      mask_on=true;
    }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;

  if(da_opt)
    {
      oldda=draw_to;
      draw_to=da_opt;
    }

  get_rects(x,y);
  if(!dr.w||!dr.h) return;

  SDL_BlitSurface(data, &sr, screen::vis, &dr);
  if(da_opt) draw_to=oldda;
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
  // FIXME: use a blitting function!

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

s_int8 image::get(SDL_RWops * file)
{
  void * tmp;
  SDL_Surface * tmp2;

  tmp=read_pnm(file,&length,&height);

  // Pitch value= Surface length * bytes_per_pixel
  tmp2=SDL_CreateRGBSurfaceFrom(tmp,length,height,24,
				length*3,0x0000FF,0x00FF00,0xFF0000,0);
  data=SDL_DisplayFormat(tmp2);
  free(tmp);
  if (!data) return(-1);
  return(0);
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

s_int8 image::load(char * fname)
{
  s_int8 res;
  res=get(fname);
  return(res);
}

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

image * image::zoom(u_int16 pixx, u_int16 pixy)
{
  u_int16 i,j;
  u_int32 temp;
  image * retimage=new image(pixx, pixy);
  retimage->data=SDL_CreateRGBSurface(SDL_SWSURFACE,
				      retimage->length,retimage->height,
				      16,0,0,0,0);
  for(j=0;j<retimage->height;j++)
    for(i=0;i<retimage->length;i++)
      {
	temp=get_pix(i*length/retimage->length,j*height/retimage->height);
	retimage->put_pix(i,j,temp);
      }
  return(retimage);
}

image * image::brightness(u_int16 cont)
{
  u_int16 i,j;
  u_int8 ir, ig, ib;
  image * retimage=new image(length, height);
  u_int32 temp=0;

  retimage->data=SDL_CreateRGBSurface(SDL_SWSURFACE,
				      retimage->length,retimage->height,
				      16,0,0,0,0);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      {
	temp=get_pix(i,j);
	SDL_GetRGB(temp,data->format,&ir,&ig,&ib);
	ir=(ir*cont)>>8;
	ig=(ig*cont)>>8;
	ib=(ib*cont)>>8;
	temp=SDL_MapRGB(data->format,ir,ig,ib);
	retimage->put_pix(i,j,temp);
      }
  return(retimage);
} 
