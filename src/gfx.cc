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
SDL_Rect image::sr;
SDL_Rect image::dr;

#ifdef SDL
SDL_Surface * screen::vis;
SDL_Surface * screen::getbuffer;
#endif

image::image ()
{
  data = NULL;
  lenght=height=0;
  mask_on=false;
  trans_on=false;
}

image::~image()
{
  free(data);
}

u_int16 image::get_lenght()
{
  return(lenght);
}

u_int16 image::get_height()
{
  return(height);
}

void image::set_sizefactor(u_int8 x)
{
  sizefactor=x;
}

#ifdef SDL
void image::putbox (u_int16 x, u_int16 y)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_mask (u_int16 x, u_int16 y)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(trans_on) { SDL_SetAlpha(data,0,0); trans_on=false; }
  if(!mask_on) 
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
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
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;

  if(mask_on) { SDL_SetColorKey(data,0,0); mask_on=false; }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, NULL, screen::vis, &dr);  
}

void image::putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			      u_int16 xo, u_int16 yo)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  if(trans_on)
    {
      SDL_SetAlpha(data,0,0);
      trans_on=false;
    }
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  x*=sizefactor;
  y*=sizefactor;
  dr.x=x;
  dr.y=y;
  dr.w=lenght;
  dr.h=height;
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
    }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, NULL, screen::vis, &dr);
}

void image::putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			       u_int16 bh, u_int16 xo, u_int16 yo, 
			       u_int8 alpha)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;

  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}


void image::putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
				    u_int16 bh, u_int16 xo, u_int16 yo, 
				    u_int8 alpha)
{
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
  sr.x=xo;
  sr.y=yo;
  sr.w=bw;
  sr.h=bh;
  dr.x=x;
  dr.y=y;
  dr.w=bw;
  dr.h=bh;
  if(!mask_on)
    {
      SDL_SetColorKey(data, SDL_SRCCOLORKEY|SDL_RLEACCEL, screen::get_trans());
      mask_on=true;
     }
  SDL_SetAlpha(data, SDL_SRCALPHA, alpha);
  trans_on=true;
  SDL_BlitSurface(data, &sr, screen::vis, &dr);
}

void image::gfxrealloc(u_int32 l)
{
 // void * tmp;
//  tmp=calloc(bytes_per_pixel,sizefactor*sizefactor*lenght*height*l);
//  data->pixels=(void*)realloc(data,l*bytes_per_pixel*sizefactor*sizefactor);
 
}

void image::size(u_int16 l, u_int16 h)
{
  if(!data)
    {  
      lenght=l;
      height=h;
    }

}

void image::resize(u_int16 l, u_int16 h)
{
  if(!data)
    {  
      lenght=l*sizefactor;
      height=h*sizefactor;
      data=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,screen::get_bytes_per_pixel()*8,0,0,0,0);
    }

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
  if(sizefactor!=1)
    {
      x*=sizefactor;
      y*=sizefactor;
      bw*=sizefactor;
      bh*=sizefactor;
      xo*=sizefactor;
      yo*=sizefactor;
    }
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
  u_int16 i,j;
  SDL_Surface *tmp;

  if(sizefactor==1) 
    {		
      tmp=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,screen::get_bytes_per_pixel()*8,0,0,0,0);
    }else{
      tmp=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,screen::get_bytes_per_pixel()*8,0,0,0,0);
    }
  
  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      
      tmp->format->Rmask=0x0000FF;
      tmp->format->Bmask=0xFF0000;
      tmp->format->Gmask=0x00FF00;
      
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      u_int8 R,G,B;
	      R=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3));
	      G=*((u_int8*)source+(j*3)+(totlen*3*i)+1+(xo*3));
	      B=*((u_int8*)source+(j*3)+(totlen*3*i)+2+(xo*3));
	      if(sizefactor==1) 
		{		
		  *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3))=R;
		  *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3)+1)=G;
		  *((u_int8*)tmp->pixels+(i*tmp->pitch)+(j*3)+2)=B;
		}else{
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6))=R;
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6)+2)=B;
		  
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp->pixels+(i*2*tmp->pitch)+(j*6)+5)=B;

		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6))=R;
		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6)+2)=B;
		  
		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp->pixels+((i*2+1)*tmp->pitch)+(j*6)+5)=B;
		}
	    }
	}
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp, tmp->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xFF00FF);
      break;

    case 2:

      u_int8 R,G,B;
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      R=*((u_int8*)source+(j*3)+(totlen*3*i)+(xo*3));
	      G=*((u_int8*)source+(j*3)+(totlen*3*i)+1+(xo*3));
	      B=*((u_int8*)source+(j*3)+(totlen*3*i)+2+(xo*3));
	      u_int32 colour = SDL_MapRGB(tmp->format, R, G, B);
	      if(sizefactor==1) 
		{
		  *((u_int16 *)tmp->pixels+(i*tmp->pitch/2)+j) = colour;
		}else{
		  *((u_int16 *)tmp->pixels+((i*2)*tmp->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp->pixels+((i*2)*tmp->pitch/2)+(j*2)+1) = colour;
		  *((u_int16 *)tmp->pixels+((i*2+1)*tmp->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp->pixels+((i*2+1)*tmp->pitch/2)+(j*2)+1) = colour;
		}
	    }
	}
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp, tmp->format, SDL_SWSURFACE);
      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xF81F);
      break;
    }
 
}

void image::putbox_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha)
{
}

void image::putbox_mask_part_img (image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh,u_int16 xo, 
				  u_int16 yo)
{
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

s_int8 image::get(FILE * file)
{
  u_int16 i,j;
  void * tmp;
  SDL_Surface * tmp2;

  tmp=read_pnm(file,&lenght,&height);

  switch(screen::get_bytes_per_pixel())
    {
    case 3:
      if(sizefactor==1) 
	{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,24,0,0,0,0);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,24,0,0,0,0);
	}

      tmp2->format->Rmask=0x0000FF;
      tmp2->format->Bmask=0xFF0000;
      tmp2->format->Gmask=0x00FF00;
      
      for(i=0;i<height;i++)
	{
	  for(j=0;j<lenght;j++)
	    {
	      u_int8 R,G,B;
	      R=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      G=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      B=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
	      
	      if(sizefactor==1) 
		{		       
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3))=R;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+1)=G;
		  *((u_int8*)tmp2->pixels+(i*tmp2->pitch)+(j*3)+2)=B;
		}else{
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+2)=B;

		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2)*tmp2->pitch)+(j*6)+5)=B;

		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6))=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+1)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+2)=B;

		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+3)=R;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+4)=G;
		  *((u_int8*)tmp2->pixels+((i*2+1)*tmp2->pitch)+(j*6)+5)=B;
		}
	    }
	}
      
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      //      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xFF00FF);
      break;
      
    case 2:
      if(sizefactor==1) 
	{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght,height,16,0,0,0,0);
	}else{
	  tmp2=SDL_CreateRGBSurface(SDL_SWSURFACE,lenght*2,height*2,16,0,0,0,0);
	}
      
      u_int8 R,G,B;
      for(i=0;i<height;i++)
	{      
	  for(j=0;j<lenght;j++)
	    {
	      R=*((u_int8*)tmp+(j*3)+(lenght*3*i));
	      G=*((u_int8*)tmp+(j*3)+(lenght*3*i)+1);
	      B=*((u_int8*)tmp+(j*3)+(lenght*3*i)+2);
	      u_int32 colour =  SDL_MapRGB(tmp2->format, R, G, B);
	      if(sizefactor==1) 
		{
		  *((u_int16 *)tmp2->pixels+(i*tmp2->pitch/2)+j) = colour;
		}else{
		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2)*tmp2->pitch/2)+(j*2)+1) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)) = colour;
		  *((u_int16 *)tmp2->pixels+((i*2+1)*tmp2->pitch/2)+(j*2)+1) = colour;
		}
	    }
	}
      lenght*=sizefactor;
      height*=sizefactor;
      data=SDL_ConvertSurface(tmp2, tmp2->format, SDL_SWSURFACE);
      //      SDL_SetColorKey(data, SDL_SRCCOLORKEY, 0xF81F);
      break;
    }
  
  free(tmp);
  
  if (!data) return(-1);
  return(0);
}
#endif

s_int8 image::load(char * fname)
{
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=get(file);
  fclose(file);
  return(res);
}

#ifdef SDL
s_int8 image::simpleget(FILE * file)
{
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
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=simpleget(file);
  fclose(file);
  return(res);
}

#ifdef SDL
void image::adapttodepth()
{
  // nothing needs to be done here, as SDL will 
  // always convert to the display surface format..
}

void screen::init_display(u_int8 vidmode = 0)
{
  u_int8 bpp;
  
  switch (vidmode)
    {
    case 0:
      screenwidth = 320;
      screenheight = 200;
      sizefactor = 1;
      image::set_sizefactor(1);
      break;
    case 1:
      screenwidth = 640;
      screenheight = 400;
      sizefactor = 2;
      image::set_sizefactor(2);
      break;
    default:                   /*printdisclaimer(); */
      exit (1);
    }

  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf (stderr, "couldn't init SDL: %s\n", SDL_GetError ());
    exit (1);
  }

  atexit (SDL_Quit);


  bpp=SDL_VideoModeOK(screenwidth, screenheight, 16, SDL_HWSURFACE);

  switch (bpp)
    {
    case 0:
      fprintf(stderr, "Video mode %dx%d unavailable. Exiting.. \n",screenwidth, screenheight);
      exit (1);
      break;
    case 16:
      printf ("Using 16bpp depth: %dx%d.\n",screenwidth,screenheight);
      bytes_per_pixel = 2;
      trans=0xF81F;  
      break;
    case 24:
      printf ("Using 24bpp depth: %dx%d.\n",screenwidth,screenheight);
      bytes_per_pixel = 3;
      trans=0xFF00FF;  
      break;
    default:
      printf ("Emulating 16bpp depth in %dbpp mode: %dx%d.\n",bpp,screenwidth,screenheight);
      bpp=16;
      bytes_per_pixel = 2;
      trans=0xF81F;
      break;
    }
 
  vis = SDL_SetVideoMode (screenwidth, screenheight, bpp, SDL_HWSURFACE);
  if (vis == NULL) 
    {
      fprintf (stderr, "error: %s\n", SDL_GetError ());
      exit (1);
    }
  // Turning off mouse cursor
  SDL_ShowCursor(0);
  
  SDL_WM_SetCaption ("Adonthell", NULL);

  init_gfx_buffers();
  frames_to_do=1;
}

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

  SDL_UpdateRect (vis, 0, 0, 0, 0);
  
  frames_to_do=timer3.tv_usec/14000;
  if (frames_to_do>20) frames_to_do=20;
}
#endif

inline u_int8 screen::get_bytes_per_pixel()
{
  return(bytes_per_pixel);
}

inline u_int32 screen::get_trans()
{
  return(trans);
}

inline u_int8 screen::get_sizefactor()
{
  return(sizefactor);
}

u_int8 screen::get_frames_to_do()
{
  return(frames_to_do);
}

u_int8 screen::get_screenheight()
{
  return(screenheight);
}

u_int8 screen::get_screenwidth()
{
  return(screenwidth);
}

#ifdef SDL
void screen::init_gfx_buffers()
{
 getbuffer=SDL_ConvertSurface(vis, vis->format, SDL_HWSURFACE);
}
#endif


void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, u_int32 color)
{
  x*=sizefactor;
  y*=sizefactor;
  w*=sizefactor;
  h*=sizefactor;

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
  drawbox(0,py,320,fact,0);
  drawbox(0,200-fact-py,320,fact,0);
  drawbox(px,0,fact,200,0);
  drawbox(320-fact-px,0,fact,200,0);
}

sprite::sprite()
{
  frame=NULL;
  nbr_of_frames=0;
}

sprite::~sprite()
{
  u_int16 i;
  for(i=0;i<nbr_of_frames;i++)
    frame[i].~image();
  free(frame);
}

void sprite::get(FILE * file)
{
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
