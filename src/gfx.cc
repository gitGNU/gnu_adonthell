// $Id$

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "types.h"
#include "gfx.h"
#include "pnm.h"

u_int16 screen::screenwidth;
u_int16 screen::screenheight;
u_int8 screen::frames_to_do;
u_int8 screen::bytes_per_pixel;
u_int8 screen::sizefactor;
u_int32 screen::trans;

u_int8 image::bytes_per_pixel, image::sizefactor;

#ifdef USE_GGI
ggi_visual_t screen::vis;
void * image::fxbuffer;
void * image::maskbuffer;
void * image::getbuffer;
#endif

#ifdef SDL
SDL_Surface * screen::vis;
SDL_Surface * screen::getbuffer;
#endif

image::image ()
{
  if(DEBUG) fprintf(stderr, "Function: image::image\n");
  data = NULL;
  lenght=height=0;
}

image::~image()
{
 if(DEBUG) fprintf(stderr, "Function: image::~image\n");
  free(data);
}

#ifdef USE_GGI
inline void image::getscr(u_int16 x, u_int16 y)
{
  ggiGetBox(screen::vis,x,y,lenght,height,getbuffer);
}

inline void image::getscr_size(u_int16 x, u_int16 y, u_int16 w, u_int16 h)
{
  ggiGetBox(screen::vis,x,y,w,h,getbuffer);
}

void image::mask(void * source=NULL)
{
  if(DEBUG) fprintf(stderr, "Function: image::mask\n");
  u_int32 i;
  u_int32 masklenght=lenght*height*
    screen::get_sizefactor()*screen::get_sizefactor();
  if(!source) source=data;
  if (screen::get_bytes_per_pixel()==2)
    for(i=0;i<masklenght;i++)
      {
	*((u_int16*)maskbuffer+i)=
	  (*((u_int16*)source+i)!=screen::get_trans())?
	  *((u_int16*)source+i):*((u_int16*)getbuffer+i);
      }
  else if (screen::get_bytes_per_pixel()==3)
    for(i=0;i<masklenght;i++)
      {
	u_int16 temp=0x00;
	u_int8 temp2=0xFF;
	u_int8 src=*((u_int8*)source+(i*3));	
	u_int8 src2=*((u_int8*)source+(i*3)+1);
	u_int8 src3=*((u_int8*)source+(i*3)+2);
	
	if ((src==temp2)
	    &&(src2==temp)&&(src3==temp2))
	  {
      	    *((u_int16*)maskbuffer+(i*3))=
	      *((u_int16*)getbuffer+(i*3));
	     *((u_int8*)maskbuffer+(i*3)+2)=
	       *((u_int8*)getbuffer+(i*3)+2);
	     
	     *((u_int8*)maskbuffer+(i*3))=*((u_int8*)getbuffer+(i*3));
	     *((u_int8*)maskbuffer+(i*3)+1)=*((u_int8*)getbuffer+(i*3)+1);
	     *((u_int8*)maskbuffer+(i*3)+2)=*((u_int8*)getbuffer+(i*3)+2);
	  }else
	  {
	    *((u_int8*)maskbuffer+(i*3))=src;
	    *((u_int8*)maskbuffer+(i*3)+1)=src2;
	    *((u_int8*)maskbuffer+(i*3)+2)=src3;
	  }
      } 
}

void image::part(u_int16 w, u_int16 h, u_int16 xo,u_int16 yo, 
		 void * dest=NULL)
{
  if(DEBUG) fprintf(stderr, "Function: image::part\n");
  u_int16 i;
  u_int16 bw=lenght;
  u_int16 bh=height;
  if(!dest) dest=getbuffer;
  w*=screen::get_sizefactor();
  h*=screen::get_sizefactor();
  bw*=screen::get_sizefactor();
  bh*=screen::get_sizefactor();
  xo*=screen::get_sizefactor();
  yo*=screen::get_sizefactor();
  for(i=0;i<h;i++)
    memcpy((char*)dest+(w*i*screen::get_bytes_per_pixel()),
 	   (char*)data+((((yo+i)*bw)+xo)*screen::get_bytes_per_pixel()),
	   w*screen::get_bytes_per_pixel()); 
}

void image::trans(u_int8 alpha, void * source=NULL)
{
  if(DEBUG) fprintf(stderr, "Function: image::trans\n");
  u_int16 i;
  u_int16 translenght=get_lenght()*get_height();
  register u_int16 destpixel;
  u_int16 r,g,b;
  if(!source) source=data;
  translenght*=screen::get_sizefactor()*screen::get_sizefactor();
  if(screen::get_bytes_per_pixel()==2)
    for (i=0;i<translenght;i++)
      { 
	r=*((u_int16*)source+i) & 63488;
	g=*((u_int16*)source+i) & 2016;
	b=*((u_int16*)source+i) & 31;
	
	destpixel=(((((*((u_int16*)getbuffer+i)
		       & 63488) -r)*alpha)>>8)&63488)+r;
	destpixel|=(((((*((u_int16*)getbuffer+i)
			& 2016) -g)*alpha)>>8)&2016)+g;
	destpixel|=(((((*((u_int16*)getbuffer+i)
			& 31) -b)*alpha)>>8)&31)+b;
	
	*((u_int16*)fxbuffer+i)=destpixel;
      }
  else if(screen::get_bytes_per_pixel()==3)
    for(i=0;i<translenght;i++)
      {
	r=*((u_int8*)source+(i*3));
	g=*((u_int8*)source+(i*3)+1);
	b=*((u_int8*)source+(i*3)+2);
	*((u_int8*)fxbuffer+(i*3))=
	  (((((*((u_int8*)getbuffer+(i*3))-r)*alpha)>>8)+r));
	*((u_int8*)fxbuffer+(i*3)+1)=
	  (((((*((u_int8*)getbuffer+(i*3)+1)-g)*alpha)>>8)+g));
	*((u_int8*)fxbuffer+(i*3)+2)=
	  (((((*((u_int8*)getbuffer+(i*3)+2)-b)*alpha)>>8)+b));
      }
}
#endif

u_int16 image::get_lenght()
{
  if(DEBUG) fprintf(stderr, "Function: image::get_lenght\n");
  return(lenght);
}

u_int16 image::get_height()
{
  if(DEBUG) fprintf(stderr, "Function: image::get_height\n");
  return(height);
}

#ifdef USE_GGI
void image::putbox (u_int16 x, u_int16 y)
{
  ggiPutBox (screen::vis, x, y, lenght, height, data);
}

void image::putbox_mask (u_int16 x, u_int16 y)
{
  getscr(x,y);
  mask();
  ggiPutBox(screen::vis,x,y,lenght,height,maskbuffer);
}

void image::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo)
{
  part(bw,bh,xo,yo);
  ggiPutBox (screen::vis, x, y, bw, bh, getbuffer);
}

void image::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  getscr(x,y);
  trans(alpha);
  ggiPutBox (screen::vis, x, y, lenght, height, fxbuffer);
}

void image::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			      u_int16 xo, u_int16 yo)
{
  static image temp;
  getscr_size(x,y,bw,bh);
  part(bw,bh,xo,yo,fxbuffer);
  temp.data=fxbuffer;
  temp.lenght=bw;
  temp.height=bh;
  temp.mask();
  ggiPutBox (screen::vis, x, y, bw, bh, maskbuffer);
}
void image::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  getscr(x,y);
  mask();
  trans(alpha,maskbuffer);
  ggiPutBox (screen::vis, x, y, lenght, height, fxbuffer);
}

void image::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			       u_int16 bh, u_int16 xo, u_int16 yo, 
			       u_int8 alpha)
{
  static image temp;
  temp.data=maskbuffer;
  temp.lenght=bw;
  temp.height=bh;
  getscr_size(x,y,bw,bh);
  part(bw,bh,xo,yo,maskbuffer);
  temp.trans(alpha);
  ggiPutBox (screen::vis, x, y, bw, bh, fxbuffer);
}


void image::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
				    u_int8 alpha)
{
  static image temp;
  temp.data=fxbuffer;
  temp.lenght=bw;
  temp.height=bh;
  getscr_size(x,y,bw,bh);
  part(bw,bh,xo,yo,fxbuffer);
  temp.mask();
  temp.trans(alpha,maskbuffer);
  ggiPutBox (screen::vis, x, y, bw, bh, fxbuffer);
}
void image::gfxrealloc(u_int32 l)
{
    data=(void*)realloc(data,l*bytes_per_pixel*sizefactor*sizefactor);
}

void image::resize(u_int16 l, u_int16 h)
{
  // Make it copy the data so the resize doesn't affect the pixels' order
  // Easier once putbox_img is written
  gfxrealloc(l*h);
  lenght=l;
  height=h;
}

void image::putbox_tile_img(image * source)
{
  u_int16 posx;
  u_int16 posy;
  for(posy=0; posy<height; posy+=source->height)
   for(posx=0; posx<lenght; posx+=source->lenght)
       putbox_img(source,posx,posy);
}

void image::putbox_img(image * source, u_int16 x, u_int16 y)
{
  u_int32 i, j;
  u_int32 l=lenght*sizefactor, h=height*sizefactor;
  u_int32 sl=source->lenght*sizefactor, sh=source->height*sizefactor;
  x*=sizefactor; y*=sizefactor;
  switch (bytes_per_pixel)
    {
    case 2:
      for(j=0;(j<sh)&&(j+y<h);j++)
	for(i=0;(i<sl)&&(i+x<l);i++)
	     *((u_int16*)data+((j+y)*l)+x+i)=
	      *((u_int16*)source->data+(j*sl)+i);
      break;
    case 3:
      // Think about a good method
      break;
    }
}

void image::putbox_mask_img (image * source, u_int16 x, u_int16 y)
{
  u_int32 i, j;
  u_int8 mltp=sizefactor*sizefactor;
  u_int32 l=lenght*mltp, h=height*mltp;
  u_int32 sl=source->lenght*mltp, sh=source->height*mltp;
  x*=mltp;y*=mltp;
  switch (screen::get_bytes_per_pixel())
    {
    case 2:
      for(j=0;(j<sh)&&(j+y<h);j++)
	for(i=0;(i<sl)&&(i+x<l);i++)
	  if((*((u_int16*)source->data+(j*sl)+i))!=screen::get_trans())
	     *((u_int16*)data+((j+y)*l)+x+i)=
	     *((u_int16*)source->data+(j*sl)+i);
      break;
    case 3:
      // Think about a good method
      break;
    }
}

void image::putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			     u_int16 bh,u_int16 xo, u_int16 yo)
{
  static image im1;
  source->part(bw,bh,xo,yo);
  im1.lenght=bw;
  im1.height=bh;
  im1.data=getbuffer;
  putbox_img(&im1,x,y);
}

void image::putbox_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha)
{
}

void image::putbox_mask_part_img (image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh,u_int16 xo, 
				  u_int16 yo)
{
  static image im1;
  source->part(bw,bh,xo,yo);
  im1.lenght=bw;
  im1.height=bh;
  im1.data=getbuffer;
  putbox_mask_img(&im1,x,y);
}

void image::putbox_mask_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int8 alpha)
{
}

void image::putbox_part_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int16 bw, u_int16 bh, u_int16 xo, 
				   u_int16 yo, u_int8 alpha)
{
}

void image::putbox_mask_part_trans_img(image * source, u_int16 x, u_int16 y, 
				       u_int16 bw, u_int16 bh,u_int16 xo, 
				       u_int16 yo, u_int8 alpha)
{
}

#endif

#ifdef SDL
void image::putbox (u_int16 x, u_int16 y)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox\n");
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_mask (u_int16 x, u_int16 y)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask\n");
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(data)
    {
      SDL_BlitSurface(data, NULL, screen::vis, &dr);
    }
}

void image::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_part\n");
  static SDL_Rect sr,dr;
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);

}

void image::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_trans\n");
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;

  if(data)
    {
      SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
      SDL_BlitSurface(data, NULL, screen::vis, &dr);
    }
}

void image::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			      u_int16 xo, u_int16 yo)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_part\n");
  static SDL_Rect sr,dr;
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;

  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_trans\n");
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;

  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			       u_int16 bh, u_int16 xo, u_int16 yo, 
			       u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_part_trans\n");
  static SDL_Rect sr,dr;
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;

  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}


void image::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
				    u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_part_trans\n");
  static SDL_Rect sr,dr;
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
 }

void image::gfxrealloc(u_int32 l)
{
  if(DEBUG) fprintf(stderr, "Function: image::gfxrealloc\n");
 // void * tmp;
//  tmp=calloc(bytes_per_pixel,sizefactor*sizefactor*lenght*height*l);
//  data->pixels=(void*)realloc(data,l*bytes_per_pixel*sizefactor*sizefactor);
 
}

void image::size(u_int16 l, u_int16 h)
{
  if(DEBUG) fprintf(stderr, "Function: image::size\n");
  if(!data)
    {  
      lenght=l;
      height=h;
    }

}

void image::resize(u_int16 l, u_int16 h)
{
  if(DEBUG) fprintf(stderr, "Function: image::resize\n");
  if(!data)
    {  
      lenght=l;
      height=h;
      data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
    }

}
void image::putbox_tile_img(image * source)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_tile_img\n");
  u_int16 posx;
  u_int16 posy;
  for(posy=0; posy<height; posy+=source->height)
   for(posx=0; posx<lenght; posx+=source->lenght)
       putbox_img(source,posx,posy);
}

void image::putbox_img(image * source, u_int16 x, u_int16 y)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_img\n");
  static SDL_Rect sr,dr;
  sr.x=0;
  sr.y=0;
  sr.w=lenght;
  sr.h=height;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  SDL_BlitSurface(source->data, &sr, data, &dr);

}

void image::putbox_mask_img (image * source, u_int16 x, u_int16 y)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_img\n");
  u_int32 i, j;
  u_int8 mltp=sizefactor*sizefactor;
  u_int32 l=lenght*mltp, h=height*mltp;
  u_int32 sl=source->lenght*mltp, sh=source->height*mltp;
  x*=mltp;y*=mltp;
  switch (screen::get_bytes_per_pixel())
    {
    case 2:
      for(j=0;(j<sh)&&(j+y<h);j++)
	for(i=0;(i<sl)&&(i+x<l);i++)
	  if((*((u_int16*)source->data->pixels+(j*sl)+i))!=screen::get_trans())
      break;
    case 3:
      // Think about a good method
      break;
    }
}

void image::putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			     u_int16 bh,u_int16 xo, u_int16 yo)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_part_img\n");
  SDL_Rect sr,dr;
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

void image::putbox_font_img (void * source, u_int16 xo, u_int16 totlen)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_font_img\n");

  u_int16 i,j;
  SDL_Surface *tmp;

  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      tmp=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
      
      tmp->format->Rmask=0x0000FF;
      tmp->format->Bmask=0xFF0000;
      tmp->format->Gmask=0x00FF00;
      
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3))=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3));
	      *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3)+1)=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3)+1);
	      *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3)+2)=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3)+2);
	    }
	}
      data=SDL_ConvertSurface(tmp, tmp->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xFF00FF);
      break;
    case 2:

      tmp=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,16,0,0,0,0);

      u_int8 R,G,B;
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      R=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3));
	      G=*((u_int8*)source+(j*3)+(totlen*3*i)+1+(xo*3));
	      B=*((u_int8*)source+(j*3)+(totlen*3*i)+2+(xo*3));
	      *((u_int16 *)tmp->pixels+(i*tmp->pitch/2)+j) = SDL_MapRGB(tmp->format, R, G, B);
	    }
	}
      data=SDL_ConvertSurface(tmp, tmp->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xF81F);
      break;
    }
 
}

void image::putbox_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_trans_img\n");
}

void image::putbox_mask_part_img (image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh,u_int16 xo, 
				  u_int16 yo)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_part_img\n");
}

void image::putbox_mask_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_trans_img\n");
}

void image::putbox_part_trans_img (image * source, u_int16 x, u_int16 y, 
				   u_int16 bw, u_int16 bh, u_int16 xo, 
				   u_int16 yo, u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_part_trans_img\n");
}

void image::putbox_mask_part_trans_img(image * source, u_int16 x, u_int16 y, 
				       u_int16 bw, u_int16 bh,u_int16 xo, 
				       u_int16 yo, u_int8 alpha)
{
  if(DEBUG) fprintf(stderr, "Function: image::putbox_mask_part_trans_img\n");
}

#endif

#ifdef USE_GGI
s_int8 image::get(FILE * file)
{
  if(DEBUG) fprintf(stderr, "Function: image::get\n");
  data=read_pnm(file,&lenght,&height);
  if (!data) return(-1);
  adapttodepth();
  return(0);
}
#endif

#ifdef SDL
s_int8 image::get(FILE * file)
{
  if(DEBUG) fprintf(stderr, "Function: image::get\n");
  u_int16 i,j;
  void * tmp;
  SDL_Surface * tmp2;

  tmp=read_pnm(file,&lenght,&height);

  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
      
      tmp2->format->Rmask=0x0000FF;
      tmp2->format->Bmask=0xFF0000;
      tmp2->format->Gmask=0x00FF00;
      
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3))=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+1)=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+2)=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
	    }
	}
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xFF00FF);
      break;
    case 2:
      tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,16,0,0,0,0);

      u_int8 R,G,B;
      for(i=0;i<height;i++)
	{      
	  for(j=0;j<lenght;j++)
	    {
	      R=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      G=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      B=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
	      *((u_int16 *)tmp2->pixels+(i*tmp2->pitch/2)+j) = SDL_MapRGB(tmp2->format, R, G, B);
	    }
	}
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xF81F);
      break;
    }

  free(tmp);
  
  if (!data) return(-1);
  return(0);
}
#endif

s_int8 image::load(char * fname)
{
  if(DEBUG) fprintf(stderr, "Function: image::load\n");
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=get(file);
  fclose(file);
  return(res);
}

#ifdef USE_GGI
s_int8 image::simpleget(FILE * file)
{
  if(DEBUG) fprintf(stderr, "Function: image::simpleget\n");
  data=read_pnm(file,&lenght,&height);
  if (data) return(0);
  return(-1);
}
#endif

#ifdef SDL
s_int8 image::simpleget(FILE * file)
{
  if(DEBUG) fprintf(stderr, "Function: image::simpleget\n");
  u_int8 bpp = screen::get_bytes_per_pixel() * 8;
  if(data) { SDL_FreeSurface(data); }
  data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,bpp,0,0,0,0);
  data->pixels=read_pnm(file,&lenght,&height);
  if (data) return(0);
  return(-1);
}
#endif

s_int8 image::simpleload(char * fname)
{
  if(DEBUG) fprintf(stderr, "Function: image::simpleload\n");
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=simpleget(file);
  fclose(file);
  return(res);
}

#ifdef USE_GGI
void image::adapttodepth()
{
  if(DEBUG) fprintf(stderr, "Function: image::adapttodepth\n");
  u_int16 i=0;
  u_int16 result;
  u_int8 temp;
  if (screen::get_bytes_per_pixel()==2)
    do
      {
      	temp=(*((u_int8*)data+(i*3)))>>3;
	result=temp<<11;
	temp=(*((u_int8*)data+(i*3)+1))>>2;
	result|=temp<<5;
	temp=(*((u_int8*)data+(i*3)+2))>>3;
	result|=temp;
	*((u_int16*)fxbuffer+i)=result;
      }while(++i<lenght*height);
  else if (screen::get_bytes_per_pixel()==3)
    do
      {
      	temp=*((u_int8*)data+(i*3)+2);
	*((u_int8*)fxbuffer+(i*3))=temp;
	*((u_int8*)fxbuffer+(i*3)+2)=*((u_int8*)data+(i*3));
	*((u_int8*)fxbuffer+(i*3)+1)=*((u_int8*)data+(i*3)+1);
      }while(++i<lenght*height);
  memcpy(data,fxbuffer,lenght*height*screen::get_bytes_per_pixel());
}
#endif

#ifdef SDL
void image::adapttodepth()
{
  if(DEBUG) fprintf(stderr, "Function: image::adapttodepth\n");
  // nothing needs to be done here, as SDL will 
  // always convert to the display surface format..
}
#endif

#ifdef USE_GGI
void screen::init_display(u_int8 vidmode = 0)
{
  u_int16 i;
  ggi_mode sgmode;
  
  switch (vidmode)
    {
    case 0:
      screenwidth = 320;
      screenheight = 200;
      sizefactor = 1;
      break;
    case 1:
      screenwidth = 640;
      screenheight = 400;
      sizefactor = 2;
      break;
    default:                   /*printdisclaimer(); */
      exit (1);
    }
  ggiInit ();
  vis = ggiOpen (NULL);
  ggiAddFlags (vis, GGIFLAG_ASYNC);
  if (!ggiCheckGraphMode (vis, screenwidth, screenheight, screenwidth, 
			  screenheight,GT_16BIT, &sgmode))
    {
      printf ("Using 16bpp depth.\n");
      i = ggiSetGraphMode (vis, screenwidth, screenheight, screenwidth, 
			   screenheight,GT_16BIT);
      if (i)
	ggiPanic ("Cannot set 320x200x16 mode!\n");
      bytes_per_pixel = 2;
      trans=0xF81F;
    }
  else if (!ggiCheckGraphMode (vis, screenwidth, screenheight, screenwidth,
			       screenheight, GT_24BIT, &sgmode))
    {
      printf ("Using 24bpp depth.\n");
      i = ggiSetGraphMode (vis, screenwidth, screenheight, screenwidth,
			   screenheight, GT_24BIT);
      if (i)
	ggiPanic ("Cannot set 320x200x24 mode!\n");
      bytes_per_pixel = 3;
      trans=0xFFFF00;
    }
  else
    {
      ggiClose (vis);
      vis = ggiOpen ("trueemu");
      ggiAddFlags (vis, GGIFLAG_ASYNC);
      if (!ggiCheckGraphMode (vis, screenwidth, screenheight, screenwidth,
			      screenheight, GT_24BIT, &sgmode))
	{
	  printf ("Using 24bpp depth.\n");
	  i = ggiSetGraphMode (vis, screenwidth, screenheight, screenwidth,
			       screenheight, GT_24BIT);
	  if (i)
	    ggiPanic ("Cannot set 320x200x24 mode!\n");
	  bytes_per_pixel = 3;
          trans=0xFFFF00;
	}
      else
	ggiPanic ("You need a display capable of 320x200 at 16 or 24bpp!\n");
    }
  ggiSetOrigin (vis, 0, 0);
  init_gfx_buffers();
  frames_to_do=1;
}
#endif

#ifdef SDL
void screen::init_display(u_int8 vidmode = 0)
{
  if(DEBUG) fprintf(stderr, "Function: screen::init_display\n");
  u_int8 bpp;
  
  switch (vidmode)
    {
    case 0:
      screenwidth = 320;
      screenheight = 200;
      sizefactor = 1;
      break;
    case 1:
      screenwidth = 640;
      screenheight = 400;
      sizefactor = 2;
      break;
    default:                   /*printdisclaimer(); */
      exit (1);
    }

  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf (stderr, "couldn't init SDL: %s\n", SDL_GetError ());
    exit (1);
  }

  atexit (SDL_Quit);


  bpp=SDL_VideoModeOK(screenwidth, screenheight, 24, SDL_HWSURFACE);

  switch (bpp)
    {
    case 0:
      fprintf(stderr, "Video mode %dx%d unavailable. Exiting.. \n",screenwidth, screenheight);
      exit (1);
      break;
    case 16:
      printf ("Using 16bpp depth.\n");
      bytes_per_pixel = 2;
      trans=0xF81F;  
      break;
    case 24:
      printf ("Using 24bpp depth.\n");
      bytes_per_pixel = 3;
      trans=0xFF00FF;  
      break;
    default:
      printf ("Emulating 16bpp depth in %dbpp mode\n",bpp);
      bpp=16;
      bytes_per_pixel = 2;
      trans=0xF81F;
      break;
    }
 
  if(sizefactor==1)
    {
      vis = SDL_SetVideoMode (screenwidth, screenheight, bpp, SDL_HWSURFACE);
      if (vis == NULL) 
	{
	  fprintf (stderr, "error: %s\n", SDL_GetError ());
	  exit (1);
	}
    }
  
  SDL_WM_SetCaption ("Adonthell", NULL);

  init_gfx_buffers();
  frames_to_do=1;
}
#endif

#ifdef USE_GGI
void screen::show()
{
  static struct timeval timer1, timer2, timer3;
#if 1
  do
    {
      gettimeofday(&timer2,NULL);
       timersub(&timer2,&timer1,&timer3);
    }while(timer3.tv_usec<14000);
#else
  struct timespec timetowait={0,0};
  gettimeofday(&timer2,NULL);
  timersub(&timer2,&timer1,&timer3);
  timetowait.tv_nsec=0;
  nanosleep(&timetowait,NULL);
#endif
  gettimeofday(&timer1,NULL);
  timer1.tv_usec-=(timer3.tv_usec%14000);
  /*  ggiPutBox(vis,0,0,320,200,screenbuffer);*/
  ggiFlush(vis);
  frames_to_do=timer3.tv_usec/14000;
  if (frames_to_do>20) frames_to_do=20;
}
#endif

#ifdef SDL
void screen::show()
{
  if(DEBUG) fprintf(stderr, "Function: screen::show\n");
  static struct timeval timer1, timer2, timer3;
#if 1
  do
    {
      gettimeofday(&timer2,NULL);
       timersub(&timer2,&timer1,&timer3);
    }while(timer3.tv_usec<14000);
#else
  struct timespec timetowait={0,0};
  gettimeofday(&timer2,NULL);
  timersub(&timer2,&timer1,&timer3);
  timetowait.tv_nsec=0;
  nanosleep(&timetowait,NULL);
#endif
  gettimeofday(&timer1,NULL);
  timer1.tv_usec-=(timer3.tv_usec%14000);

  SDL_UpdateRect (vis, 0, 0, 0, 0);
  
  frames_to_do=timer3.tv_usec/14000;
  if (frames_to_do>20) frames_to_do=20;
}
#endif

inline u_int8 screen::get_bytes_per_pixel()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_bytes_per_pixel\n");
  return(bytes_per_pixel);
}

inline u_int32 screen::get_trans()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_trans\n");
  return(trans);
}

inline u_int8 screen::get_sizefactor()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_sizefactor\n");
  return(sizefactor);
}

u_int8 screen::get_frames_to_do()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_frames_to_do\n");
  return(frames_to_do);
}

u_int8 screen::get_screenheight()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_screenheight\n");
  return(screenheight);
}

u_int8 screen::get_screenwidth()
{
  if(DEBUG) fprintf(stderr, "Function: screen::get_screenwidth\n");
  return(screenwidth);
}

#ifdef USE_GGI
void screen::init_gfx_buffers()
{
  image::fxbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
  image::maskbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
  image::getbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
}
#endif

#ifdef SDL
void screen::init_gfx_buffers()
{
 if(DEBUG) fprintf(stderr, "Function: screen::init_gfx_buffers\n");
 getbuffer=SDL_ConvertSurface(vis, vis->format, SDL_HWSURFACE);
}
#endif


void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, u_int32 color)
{
  if(DEBUG) fprintf(stderr, "Function: screen::drawbox\n");
  x*=sizefactor;
  y*=sizefactor;
  w*=sizefactor;
  h*=sizefactor;
#ifdef USE_GGI
  ggiSetGCForeground(vis,color);
  ggiDrawBox(vis,x,y,w,h);
#endif

#ifdef SDL
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=w;
  dr.h=h;
  SDL_FillRect(vis, &dr, color);
#endif
}


void screen::makesquare(u_int16 px,u_int16 py, u_int16 fact)
{
  if(DEBUG) fprintf(stderr, "Function: screen::makesquare\n");
  drawbox(0,py,320,fact,0);
  drawbox(0,200-fact-py,320,fact,0);
  drawbox(px,0,fact,200,0);
  drawbox(320-fact-px,0,fact,200,0);
}

sprite::sprite()
{
  if(DEBUG) fprintf(stderr, "Function: sprite::sprite\n");
  frame=NULL;
  nbr_of_frames=0;
}

sprite::~sprite()
{
  if(DEBUG) fprintf(stderr, "Function: sprite::~sprite\n");
  u_int16 i;
  for(i=0;i<nbr_of_frames;i++)
    frame[i].~image();
  free(frame);
}

void sprite::get(FILE * file)
{
  if(DEBUG) fprintf(stderr, "Function: sprite::get\n");
  fread(&nbr_of_frames,sizeof(nbr_of_frames),1,file);
  frame=(image*)calloc(sizeof(image),
		       nbr_of_frames);
  fread(&counterlimit,sizeof(counterlimit),1,file);
  fread(&alpha,sizeof(alpha),1,file);
  fread(&nbr_of_frames,sizeof(nbr_of_frames),1,file);  
  //  toplayername=(lstr*)calloc(sizeof(lstr),
  //			     toplayer.nbr_of_frames);
  //  for(i=0;i<amap->toplayer.nbr_of_frames;i++)
  //    {
  //      getstringfromfile(amap->toplayername[i],file);
  //      amap->toplayer.pixmap[i]=LoadGfxAdapt(amap->toplayername[i],
  //					    &amap->toplayer.lenght,
  //					    &amap->toplayer.height);
  //    }
}
