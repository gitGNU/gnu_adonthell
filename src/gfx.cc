#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "pnm.h"

ggi_visual_t screen::vis;

u_int16 screen::screenwidth;
u_int16 screen::screenheight;
u_int8 screen::frames_to_do;
u_int8 screen::bytes_per_pixel;
u_int8 screen::sizefactor;
u_int32 screen::trans;

void * image::fxbuffer;
void * image::maskbuffer;
void * image::getbuffer;

u_int8 image::bytes_per_pixel, image::sizefactor;

image::image ()
{
  data = NULL;
  lenght=height=0;
}

image::~image()
{
  free(data);
}

inline void image::getscr(u_int16 x, u_int16 y)
{
  ggiGetBox(screen::vis,x*sizefactor,y*sizefactor,
	    lenght*sizefactor,height*sizefactor,getbuffer);
}

inline void image::getscr_size(u_int16 x, u_int16 y, u_int16 w, u_int16 h)
{
  ggiGetBox(screen::vis,x*sizefactor,y*sizefactor,
	    w*sizefactor,h*sizefactor,getbuffer);
}

void image::mask(void * source=NULL)
{
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
  u_int16 i;
  u_int16 bw=lenght;
  u_int16 bh=height;
  if(!dest) dest=getbuffer;
  w*=sizefactor;
  h*=sizefactor;
  bw*=sizefactor;
  bh*=sizefactor;
  xo*=sizefactor;
  yo*=sizefactor;
  for(i=0;i<h;i++)
    memcpy((u_int8*)dest+(w*i*screen::get_bytes_per_pixel()),
 	   (u_int8*)data+((((yo+i)*bw)+xo)*screen::get_bytes_per_pixel()),
	   w*screen::get_bytes_per_pixel()); 
}

void image::trans(u_int8 alpha, void * source=NULL)
{
  u_int32 i;
  u_int32 translenght=lenght*height;
  register u_int16 destpixel;
  u_int16 r,g,b;
  if(!source) source=data;
  translenght*=sizefactor*sizefactor;
  if(screen::get_bytes_per_pixel()==2)
    for (i=0;i<translenght;i++)
      { 
	r=*((u_int16*)getbuffer+i) & 63488;
	g=*((u_int16*)getbuffer+i) & 2016;
	b=*((u_int16*)getbuffer+i) & 31;
	
	destpixel=(((((*((u_int16*)source+i)
		       & 63488) -r)*alpha)>>8)&63488)+r;
	destpixel|=(((((*((u_int16*)source+i)
			& 2016) -g)*alpha)>>8)&2016)+g;
	destpixel|=(((((*((u_int16*)source+i)
			& 31) -b)*alpha)>>8)&31)+b;
	
	*((u_int16*)fxbuffer+i)=destpixel;
      }
  else if(screen::get_bytes_per_pixel()==3)
    for(i=0;i<translenght;i++)
      {
	r=*((u_int8*)getbuffer+(i*3));
	g=*((u_int8*)getbuffer+(i*3)+1);
	b=*((u_int8*)getbuffer+(i*3)+2);
	*((u_int8*)fxbuffer+(i*3))=
	  (((((*((u_int8*)source+(i*3))-r)*alpha)>>8)+r));
	*((u_int8*)fxbuffer+(i*3)+1)=
	  (((((*((u_int8*)source+(i*3)+1)-g)*alpha)>>8)+g));
	*((u_int8*)fxbuffer+(i*3)+2)=
	  (((((*((u_int8*)source+(i*3)+2)-b)*alpha)>>8)+b));
      }
}

u_int16 image::get_lenght()
{
  return(lenght);
}

u_int16 image::get_height()
{
  return(height);
}

void image::putbox (u_int16 x, u_int16 y)
{
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     lenght*sizefactor, height*sizefactor, data);
}

void image::putbox_mask (u_int16 x, u_int16 y)
{
  getscr(x,y);
  mask();
  ggiPutBox(screen::vis,x*sizefactor,y*sizefactor,
	    lenght*sizefactor,height*sizefactor,maskbuffer);
}

void image::putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo)
{
  part(bw,bh,xo,yo);
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     bw*sizefactor, bh*sizefactor, getbuffer);
}

void image::putbox_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  getscr(x,y);
  trans(alpha);
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     lenght*sizefactor, height*sizefactor, fxbuffer);
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
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     bw*sizefactor, bh*sizefactor, maskbuffer);
}
void image::putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha)
{
  getscr(x,y);
  mask();
  trans(alpha,maskbuffer);
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     lenght*sizefactor, height*sizefactor, fxbuffer);
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
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     bw*sizefactor, bh*sizefactor, fxbuffer);
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
  ggiPutBox (screen::vis, x*sizefactor, y*sizefactor, 
	     bw*sizefactor, bh*sizefactor, fxbuffer);
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

s_int8 image::get(FILE * file)
{
  data=read_pnm(file,&lenght,&height);
  if (!data) return(-1);
  adapttodepth();
  return(0);
}

s_int8 image::load(char * fname)
{
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=get(file);
  fclose(file);
  return(res);
}

s_int8 image::simpleget(FILE * file)
{
  data=read_pnm(file,&lenght,&height);
  if (data) return(0);
  return(-1);
}

s_int8 image::simpleload(char * fname)
{
  s_int8 res;
  FILE * file=fopen(fname,"r");
  if(!file) return(-1);
  res=simpleget(file);
  fclose(file);
  return(res);
}

void image::adapttodepth()
{
  u_int16 i=0,j;
  u_int16 result;
  u_int8 temp;
  u_int8 bytes_per_pixel=screen::get_bytes_per_pixel();
  u_int8 sizefactor=screen::get_sizefactor();
  void * buf=calloc(screen::get_bytes_per_pixel(),1);

  if (bytes_per_pixel==2)
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
  else if (bytes_per_pixel==3)
    do
      {
      	temp=*((u_int8*)data+(i*3)+2);
	*((u_int8*)fxbuffer+(i*3))=temp;
	*((u_int8*)fxbuffer+(i*3)+2)=*((u_int8*)data+(i*3));
	*((u_int8*)fxbuffer+(i*3)+1)=*((u_int8*)data+(i*3)+1);
      }while(++i<lenght*height);
  resize(lenght,height);
  memcpy(data,fxbuffer,lenght*height*bytes_per_pixel);

  // Size adaptation

  if(sizefactor==1) return;
  switch(bytes_per_pixel)
    {
    case 2:
      if(sizefactor==2)
	{
	  //      	  for(i=0;i<lenght*height*sizefactor*sizefactor*bytes_per_pixel;i++)
	  //	    *((u_int16*)fxbuffer+i)=0x0;
	  for(j=0;j<height;j++)
	    for(i=0;i<lenght;i++)
	      {
		*((u_int16*)buf)=*((u_int16*)data+(j*lenght)+i);
		*((u_int16*)fxbuffer+((j*lenght*4)+(i*2)))=*((u_int16*)buf);
		*((u_int16*)fxbuffer+((j*lenght*4)+(i*2))+1)=*((u_int16*)buf);
		*((u_int16*)fxbuffer+((j*lenght*4)+(i*2))+(lenght*2))=*((u_int16*)buf);
		*((u_int16*)fxbuffer+((j*lenght*4)+(i*2))+(lenght*2)+1)=*((u_int16*)buf);
	      }
	}
      break;
    case 3:
      if(sizefactor==2)
	for(j=0;j<height;j++)
	  for(i=0;i<lenght;i++)
	    {
	      memcpy(buf,(u_int8*)data+((j*lenght)+i)*bytes_per_pixel,bytes_per_pixel);
	      memcpy((u_int8*)fxbuffer+(((j*lenght*4)+(i*2)))*bytes_per_pixel,buf,bytes_per_pixel);
	      memcpy((u_int8*)fxbuffer+(((j*lenght*4)+(i*2))+1)*bytes_per_pixel,buf,bytes_per_pixel);
	      memcpy((u_int8*)fxbuffer+(((j*lenght*4)+(i*2))+(lenght*2))*bytes_per_pixel,buf,bytes_per_pixel);
	      memcpy((u_int8*)fxbuffer+(((j*lenght*4)+(i*2))+(lenght*2)+1)*bytes_per_pixel,buf,bytes_per_pixel);
	    }
      break;
    }
    memcpy(data,fxbuffer,lenght*height*bytes_per_pixel*sizefactor*sizefactor);
}

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

void screen::show()
{
  static struct timeval timer1, timer2, timer3;
  static struct timezone obso;
#if 1
  do
    {
      gettimeofday(&timer2,&obso);
       timersub(&timer2,&timer1,&timer3);
    }while(timer3.tv_usec<14000);
#else
  struct timespec timetowait={0,0};
  gettimeofday(&timer2,&obso);
  timersub(&timer2,&timer1,&timer3);
  timetowait.tv_nsec=0;
  nanosleep(&timetowait,NULL);
#endif
  gettimeofday(&timer1,&obso);
  timer1.tv_usec-=(timer3.tv_usec%14000);
  /*  ggiPutBox(vis,0,0,320,200,screenbuffer);*/
  ggiFlush(vis);
  frames_to_do=timer3.tv_usec/14000;
  if (frames_to_do>20) frames_to_do=20;
}

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

void screen::init_gfx_buffers()
{
  image::fxbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
  image::maskbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
  image::getbuffer=calloc(bytes_per_pixel,screenwidth*screenheight);  
  image::bytes_per_pixel=bytes_per_pixel;
  image::sizefactor=sizefactor;
}

void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, u_int32 color)
{
  x*=sizefactor;
  y*=sizefactor;
  w*=sizefactor;
  h*=sizefactor;
  ggiSetGCForeground(vis,color);
  ggiDrawBox(vis,x,y,w,h);
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
