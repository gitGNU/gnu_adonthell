#ifndef _gfx_h
#define _gfx_h

#ifdef SDL
#include "types.h"
#include <SDL/SDL.h>
#endif

#define DEBUG 0

class screen
{
  static u_int16 screenwidth;
  static u_int16 screenheight;
  static u_int8 frames_to_do;
  static u_int8 bytes_per_pixel;
  static u_int8 sizefactor;
  static u_int32 trans;
 public:

#ifdef SDL
  static SDL_Surface * vis;
  static SDL_Surface * getbuffer;
#endif

  static void init_display(u_int8 vidmode = 0);
  static void show();
  static u_int8 get_bytes_per_pixel();
  static u_int8 get_sizefactor();
  static u_int32 get_trans();
  static u_int8 get_frames_to_do();
  static u_int8 get_screenwidth();
  static u_int8 get_screenheight();
  static void init_gfx_buffers();
  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color);
  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
};

class image
{
 protected:
  u_int16 lenght, height;
  static u_int8 bytes_per_pixel, sizefactor;
  static SDL_Rect sr,dr;

#ifdef SDL
  SDL_Surface * data;
  bool mask_on;
  bool trans_on;
#endif

 public:
  image();
  ~image();
  u_int16 get_lenght();
  u_int16 get_height();
  static void set_sizefactor(u_int8 x);
  static u_int8 get_sizefactor();
  void gfxrealloc(u_int32 l);
  void putbox (u_int16 x, u_int16 y);
  void putbox_mask (u_int16 x, u_int16 y);
  void putbox_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
		    u_int16 xo, u_int16 yo);
  void putbox_trans (u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_mask_part (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			 u_int16 xo, u_int16 yo);
  void putbox_mask_trans (u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_part_trans (u_int16 x, u_int16 y, u_int16 bw, 
			  u_int16 bh, u_int16 xo, u_int16 yo, 
			  u_int8 alpha);
  void putbox_mask_part_trans (u_int16 x, u_int16 y, u_int16 bw, u_int16 bh,
			       u_int16 xo, u_int16 yo, u_int8 alpha);
  void resize(u_int16 l, u_int16 h);
  void size(u_int16 l, u_int16 h);
  // Same as the top seen putbox functions, but act on the image's data instead
  // of the screen. The first parameter is the image to draw on the image's 
  // data
  void putbox_tile_img(image * source);
  void putbox_img(image * source, u_int16 x, u_int16 y);
  void putbox_mask_img (image * source, u_int16 x, u_int16 y);
  void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			u_int16 bh, u_int16 xo, u_int16 yo);
  void putbox_font_img (void * source, u_int16 xo, u_int16 totlen);
  void putbox_trans_img (image * source, u_int16 x, u_int16 y, u_int8 alpha);
  void putbox_mask_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw,
			     u_int16 bh, u_int16 xo, u_int16 yo);
  void putbox_mask_trans_img (image * source, u_int16 x, u_int16 y, 
			      u_int8 alpha);
  void putbox_part_trans_img (image * source, u_int16 x, u_int16 y, u_int16 bw,
 			      u_int16 bh, u_int16 xo, u_int16 yo, 
			      u_int8 alpha);
  void putbox_mask_part_trans_img(image * source, u_int16 x, u_int16 y, 
				  u_int16 bw, u_int16 bh, u_int16 xo, 
				  u_int16 yo, u_int8 alpha);
  s_int8 get(FILE * file);
  s_int8 load(char * fname);
  s_int8 simpleget(FILE * file);
  s_int8 simpleload(char * fname);
  void adapttodepth();
  friend void screen::init_gfx_buffers();
};

class sprite
{
  image * frame;
  u_int8 nbr_of_frames;          // u_int16!!
  u_int8 counterlimit;
  u_int8 currentframe;
  u_int8 alpha;
 public:
  sprite();
  ~sprite();
  void get(FILE * file);
};

#endif
