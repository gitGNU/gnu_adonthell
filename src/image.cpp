// $Id$
/*
   Copyright (C) 1999-2000   The Adonthell Project
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
#include "types.h"
#include "image.h"

SDL_Rect image::sr;
SDL_Rect image::dr;
#ifdef _DEBUG_
u_int16 image::a_d_diff=0;
#endif

image &image::operator =(const image &im)
{
  length=im.length;
  height=im.height;
  draw_to=im.draw_to;
  bytes_per_pixel=im.bytes_per_pixel;
  mask_on=im.mask_on;
  alpha=im.alpha;
  if(data) SDL_FreeSurface(data);
#ifdef _EDIT_
  if(simpledata) free(simpledata);
#endif
  data=SDL_ConvertSurface(im.data,im.data->format,im.data->flags);
#ifdef _EDIT_
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
  set_alpha(255);
  draw_to=NULL;
#ifdef _EDIT_
  simpledata=NULL;
#endif
}

image::image ()
{
#ifdef _DEBUG_
  cout << "image() called, " << ++a_d_diff
       << " objects currently allocated\n";
#endif
  init();
}

image::image (u_int16 l, u_int16 h)
{
#ifdef _DEBUG_
  cout << "image(u_int16, u_int16) called, "<< ++a_d_diff
       << " objects currently allocated\n";
#endif
  init();
  length=l;
  height=h;
  bytes_per_pixel=screen::bytes_per_pixel;
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,length, height,
			    bytes_per_pixel*8,0,0,0,0);
#ifdef _EDIT_
  simpledata=(void*)calloc(length*height,3);
#endif
}

image::~image()
{
  clear();
#ifdef _DEBUG_
  cout << "~image() called, " << --a_d_diff
       << " objects currently allocated\n";
#endif
}

void image::clear()
{
  SDL_FreeSurface(data);
  data=NULL;
#ifdef _EDIT_
  free(simpledata);
#endif
}

u_int16 image::get_length()
{
  return length;
}

u_int16 image::get_height()
{
  return height;
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
#ifdef _EDIT_
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
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, screen::trans);
      mask_on=true;
    }
  else if((!m)&&(mask_on))
    { 
      SDL_SetColorKey(data,0,0); 
      mask_on=false;    
      }
}

u_int8 image::get_alpha()
{
#ifdef REVERSE_ALPHA
  return alpha;
#else
  return 255-alpha;
#endif
}    

void image::set_alpha(u_int8 t)
{
#ifdef REVERSE_ALPHA
  if((t==255)&&(alpha!=255)&&data) SDL_SetAlpha(data,0,0);
  alpha=t;
#else
  if((t==0)&&(alpha!=0)&&data) SDL_SetAlpha(data,0,0);
  alpha=255-t;
#endif
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
  if(get_alpha()!=255) SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
  if(da_opt) draw_to=oldda;  
}

void image::putbox (s_int16 x, s_int16 y, drawing_area * da_opt)
{  
  if(data==NULL) return;
  set_mask(false);
  set_alpha(255);
  
  draw(x,y,da_opt);
}

void image::putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(true);
  set_alpha(255);

  draw(x,y,da_opt);
}

void image::putbox_trans (s_int16 x, s_int16 y, u_int8 a, 
			  drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(false);
  set_alpha(a);

  draw(x,y,da_opt);
}

void image::putbox_mask_trans (s_int16 x, s_int16 y, u_int8 a, 
			       drawing_area * da_opt)
{
  if(data==NULL) return;
  set_mask(true);
  set_alpha(a);

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
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, screen::trans);
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

s_int8 image::get(gzFile file)
{
  s_int8 ret;
  gzread(file,&mask_on,sizeof(mask_on));
  gzread(file,&alpha,sizeof(alpha));   
  ret=get_raw(file);
  if(!ret)
    {
      if(mask_on)
	SDL_SetColorKey(data, SDL_SRCCOLORKEY, screen::trans);
      set_alpha(alpha);
    }
  return ret;
}

s_int8 image::load(const char * fname)
{
  gzFile file;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(1);
  get(file);
  gzclose(file);
  return(0);
}

s_int8 image::get_raw(gzFile file)
{
#ifndef _EDIT_
  void * simpledata;
#endif
  SDL_Surface * tmp2;
  gzread(file,&length,sizeof(length));
  gzread(file,&height,sizeof(height));
  simpledata=calloc(length*height,3);
  gzread(file,simpledata,length*height*3);

  tmp2=SDL_CreateRGBSurfaceFrom(simpledata,length,height,24,
				length*3,0x0000FF,0x00FF00,0xFF0000,0);
  data=SDL_DisplayFormat(tmp2);
  bytes_per_pixel=screen::bytes_per_pixel;
  SDL_FreeSurface(tmp2);

#ifndef _EDIT_
  free(simpledata);
#endif
  if (!data) return(-1);
  return(0);
}

s_int8 image::load_raw(const char * fname)
{
  gzFile file;
  file=gzopen(fname,"rb"); 
  if(!file) return(1);
  get_raw(file);
  gzclose(file);
  return(0);
}

s_int8 image::get_pnm(SDL_RWops * file)
{
#ifndef _EDIT_
  void * simpledata;
#endif
  SDL_Surface * tmp2;
  simpledata=read_pnm(file,&length,&height);

  tmp2=SDL_CreateRGBSurfaceFrom(simpledata,length,height,24,
				length*3,0x0000FF,0x00FF00,0xFF0000,0);
  data=SDL_DisplayFormat(tmp2);
  bytes_per_pixel=screen::bytes_per_pixel;
  SDL_FreeSurface(tmp2);

#ifndef _EDIT_
  free(simpledata);
#endif
  if (!data) return(-1);
  return(0);
}

s_int8 image::load_pnm(const char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"r"); 
  if(!file) return(1);
  get_pnm(file);
  SDL_RWclose(file);
  return(0);
}

#ifdef _EDIT_
s_int8 image::put(gzFile file)
{
  gzwrite(file,&mask_on,sizeof(mask_on));
  set_alpha(alpha);
  gzwrite(file,&alpha,sizeof(alpha));
  set_alpha(alpha);
  return(put_raw(file));
}

s_int8 image::save(char * fname)
{
  gzFile file;
  file=gzopen(fname,"wb6"); 
  if(!file) return(1);
  put(file);
  gzclose(file);
  return(0);
}

s_int8 image::put_raw(gzFile file)
{
  gzwrite(file,&length,sizeof(length));
  gzwrite(file,&height,sizeof(height));
  gzwrite(file,simpledata,length*height*3);
  return(0);
}

s_int8 image::save_raw(char * fname)
{
  gzFile file;
  file=gzopen(fname,"w6"); 
  if(!file) return(1);
  put_raw(file);
  gzclose(file);
  return(0);
}

s_int8 image::put_pnm(SDL_RWops * file)
{
  pnmput(file,simpledata,length,height);
  return(0);
}

s_int8 image::save_pnm(char * fname)
{
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"w"); 
  if(!file) return(1);
  put_pnm(file);
  SDL_RWclose(file);
  return(0);
}

#endif

u_int32 image::get_pix(u_int16 x, u_int16 y)
{
  static u_int32 retvalue;

#ifndef _EDIT_
  const u_int32 offset=((y*((length%2?length+1:length)))+x);
  switch (bytes_per_pixel)
    {
    case 1: return *((u_int8*)data->pixels+offset);
      break;
    case 2: return *((u_int16*)data->pixels+offset);
      break;
    case 3: return *((u_int32*)data->pixels+offset);
      break;
    default: memcpy(&retvalue,(char*)data->pixels+(offset*bytes_per_pixel),
		    bytes_per_pixel);
      return retvalue;
      break;
    }
#else
  memcpy(&retvalue,(char*)simpledata+((x+(y*length))*3),3);
  return(retvalue);
#endif
}

void image::put_pix(u_int16 x, u_int16 y, u_int32 col)
{
  const u_int32 offset=((y*((length%2?length+1:length)))+x);

#ifdef _EDIT_
  u_int8 r,g,b; 
  char * c;
  c=(char*)&col;
  memcpy(&r,c,1);
  memcpy(&g,c+1,1);
  memcpy(&b,c+2,1);
  memcpy((char*)simpledata+((x+(y*length))*3),&col,3);
  /*  col=SDL_MapRGB(data->format,*(char*)(&col+2),
		 *(char*)(&col+2),
		 *(char*)(&col+2));*/
  col=SDL_MapRGB(data->format,r,g,b);
#endif

  switch (bytes_per_pixel)
    {
    case 1: *((u_int8*)data->pixels+offset)=(u_int8)col;
      break;
    case 2: *((u_int16*)data->pixels+offset)=(u_int16)col;
      break;
    case 4: *((u_int32*)data->pixels+offset)=(u_int32)col;
      break;
    default: memcpy((char*)data->pixels+(offset*bytes_per_pixel),&col,
		    bytes_per_pixel);
      break;
    }
}

void image::zoom(image * src)
{
  static u_int16 i,j;
  SDL_LockSurface(src->data);
  SDL_LockSurface(data);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
       	put_pix(i,j,src->get_pix(i*src->length/length,j*src->height/height));
  SDL_UnlockSurface(src->data);
  SDL_UnlockSurface(data);
}

void image::reverse_lr(image * src)
{
  static u_int16 i,j;
  if((length!=src->length)||(height!=src->height))
    resize(src->length,src->height);
  SDL_LockSurface(src->data);
  SDL_LockSurface(data);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      {
	//	cout << src->get_pix(i,j) << endl;
	put_pix(length-i,j,src->get_pix(i,j));
      }
  SDL_UnlockSurface(src->data);
  SDL_UnlockSurface(data);
}

void image::reverse_ud(image * src)
{
  static u_int16 i,j;
  if((length!=src->length)||(height!=src->height))
    resize(src->length,src->height);
  SDL_LockSurface(src->data);
  SDL_LockSurface(data);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      put_pix(i,height-(j+1),src->get_pix(i,j));
  SDL_UnlockSurface(src->data);
  SDL_UnlockSurface(data);
}

void image::brightness(image * src, u_int16 cont, bool proceed_mask=false)
{
  u_int16 i,j;
  u_int8 ir, ig, ib;
  u_int32 temp=0;
  
  resize(src->length,src->height);
  for(j=0;j<height;j++)
    for(i=0;i<length;i++)
      {
	temp=src->get_pix(i,j);
	if((proceed_mask) || temp!=screen::trans_pix)
	  {
	    SDL_GetRGB(temp,src->data->format,&ir,&ig,&ib);
	    ir=(ir*cont)>>8;
	    ig=(ig*cont)>>8;
	    ib=(ib*cont)>>8;
	    temp=SDL_MapRGB(data->format,ir,ig,ib);
	  }
	put_pix(i,j,temp);
      }
}
