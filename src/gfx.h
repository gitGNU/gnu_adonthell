#ifndef _gfx_h
#define _gfx_h

#include "ggi.h"
#include "types.h"

// All the members of this class are static. It's no use to declare a
// screen object - use screen:<function> instead.
class screen
{
public:
  // Screen resolution
  static u_int16 screenwidth;
  static u_int16 screenheight;
private:
  // Number of frames to calculate before displaying
  static u_int8 frames_to_do;

  // Number of bytes per image pixel
  static u_int8 bytes_per_pixel;

  // Image size factor - multiply the displayed size of each image
  // (and the size in memory too!)
  static u_int8 sizefactor;

  // Static value that contains the value of the transparent pixels
  static u_int32 trans;

 public:
  static ggi_visual_t vis;
  static void init_display(u_int8 vidmode = 0);
  static void show();
  static u_int8 get_bytes_per_pixel();
  static u_int8 get_sizefactor();
  static u_int32 get_trans();
  static u_int8 get_frames_to_do();

  // image class friend
  static void init_gfx_buffers();

  // single color box drawing
  static void drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		      u_int32 color);

  // Transition fx, when entering a house for example
  static void makesquare(u_int16 px, u_int16 py, u_int16 factor);
};


// Base class for each graphical element
class image
{
  void * data;
  u_int16 lenght, height;
  static u_int8 bytes_per_pixel, sizefactor;
  static void * fxbuffer;
  static void * maskbuffer;
  static void * getbuffer;
  // these private functions are not intented to be used directly, excepted
  // perhaps in very specific cases
  void copy(void * target=NULL);
  // copy the entire image into target, or getbuffer if not specified
  void getscr(u_int16 x, u_int16 y);
  // get a part of the current screen and put it into getbuffer
  // the size of the part is the same than the image
  void getscr_size(u_int16 x, u_int16 y, u_int16 w, u_int16 h);
  // get a part of the current screen of specified size and put it into 
  // getbuffer  
  void mask(void * source=NULL);
  // mask the image with getbuffer and put the result into maskbuffer 
  // if source is not specified, it will by default point to the data of
  // the image
  void part(u_int16 bw, u_int16 bh, u_int16 xo, u_int16 yo, void * dest=NULL);
  // cut a part of the image and put it into dest
  // if dest is not specified, it will by default point getbuffer 
  void trans(u_int8 alpha, void * source=NULL);
  // apply a translucency on the image with getbuffer, put the result into
  // fxbuffer
  // if source is not specified, it will by default point to the data of 
  // the image

  // depth adaptation function
  void adapttodepth();

 public:
  image();
  ~image();
  u_int16 get_lenght();
  u_int16 get_height();

  void gfxrealloc(u_int32 l);

  // Image display functions
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

  // Image buffer work functions
  void resize(u_int16 l, u_int16 h);
  // Same as the top seen putbox functions, but act on the image's data instead
  // of the screen. The first parameter is the image to draw on the image's 
  // data
  void putbox_tile_img(image * source);
  void putbox_img(image * source, u_int16 x, u_int16 y);
  void putbox_mask_img (image * source, u_int16 x, u_int16 y);
  void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw, 
			u_int16 bh, u_int16 xo, u_int16 yo);
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
  // Image loading functions
  // Which take care of the graphical depth
  s_int8 get(FILE * file);
  s_int8 load(char * fname);
  // Doesn't take care of the graphical depth
  s_int8 simpleget(FILE * file);
  s_int8 simpleload(char * fname);

  // friend function that is called at screen initialization time to init
  // the static graphic buffers.
  friend void screen::init_gfx_buffers();
};

// Still unused - class for size-variable, complicated animations
// Have to be rethough...
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
