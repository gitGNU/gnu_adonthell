/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/



/** @file image.h
 *  Declares the image class.
 */ 


#ifndef _image_h
#define _image_h
 
#include "fileops.h"
#include "screen.h"
#include <zlib.h>
#include "pnm.h" 


/** Image manipulation class.
 *  Designed to work with single images, without having to care about the bit
 *  depth. This class is widely used through the %game - in fact it handles
 *  everything that is displayed on the %screen.
 *  Each image has two parameters - a mask parameter, defining whether the
 *  transparent color (which hex triplet is FF00FF) should be displayed or no,
 *  allowing to display images that doesn't look like squares, and an alpha
 *  parameter that define the opacity of the image. It's a 8 bits large
 *  unsigned integer, so correct values for it go from 0 (invisible) to 255
 *  (totally opaque).
 *  When compiled with the _EDIT_ flag, image provides lots of other methods
 *  for image editing and saving.
 */
class image
{
 public:

    /** Default constructor.
     */ 
    image ();
    
#ifndef SWIG
    /** Creates an image with a specified size.
     *  @param l length of the image.
     *  @param h height of the image.
     */ 
    image (u_int16 l, u_int16 h);
#endif

    /** Destructor
     */ 
    ~image ();

    /** Totally clears the image.
     *  The image is resetted as if it were just created with the
     *  default constructor.
     *  @sa image ()
     */ 
    void clear ();

    /** Resize the image.
     *  The content of the image is destroyed. If you want to zoom an image,
     *  see rather the zoom () function.
     *  @param l new image length.
     *  @param h new image height.
     *  @sa zoom ()
     */ 
    void resize (u_int16 l, u_int16 h);

    /** Returns the length of the image.
     *  @return the image's length.
     *  @sa height ()
     */ 
    u_int16 length ()
    {
        return length_;
    }
    
    /** Returns the height of the image.
     *  @return the image's height.
     *  @sa length () 
     */ 
    u_int16 height ()
    {
        return height_;
    }
    
    /** Return whether the image is masked or not.
     *  @return
     *     - false: not masked.
     *     - true: masked.
     *  @sa set_mask () 
     */ 
    bool is_masked ()
    {
        return (mask_on); 
    }

    /** Set the mask value for this image.
     *  @param m
     *     - false: image isn't masked.
     *     - true: image is masked.
     *  @sa is_masked ()
     */ 
    void set_mask (bool m);

    /** Return the alpha value for this image.
     *  @return the image's alpha value.
     *  @sa set_alpha () 
     */ 
    u_int8 alpha ()
    {
#ifdef REVERSE_ALPHA
        return alpha_;
#else
        return 255 - alpha_;
#endif
    }

    /** Sets the image's alpha value.
     *  @param t the image's new alpha value.
     *  @sa alpha () 
     */ 
    void set_alpha (u_int8 t);

    /** Loads an image from an opened file, saved in %game internal format,
     *  with alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa load ()
     */ 
    s_int8 get (igzstream& file);

    /** Loads an image from a file name, with alpha and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa get ()
     */
    s_int8 load (string fname);

    /** Loads an image from an opened file, saved in %game internal format,
     *  without alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa load_raw () 
    */
    s_int8 get_raw (igzstream& file);
    
    /** Loads an image from a file name, without alpha and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa get_raw ()
     */ 
    s_int8 load_raw (string fname);

    /** Loads an image from an opened file, in PNM format, without
     *  alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa load_pnm () 
    */
    s_int8 get_pnm (SDL_RWops * file);
    
    /** Loads an image from a file name, in PNM format, without
     *  alpha and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa get_pnm ()
     */ 
    s_int8 load_pnm (string fname);
 
    /** Assign a drawing_area to this image.
     *  @param da the drawing area to assign.
     */ 
    void assign_drawing_area (drawing_area * da);

    /** Detach the drawing_area attached to the image (if any).
     */
    void detach_drawing_area ();

    /** Draw the image to the %screen.
     *  @param x X position.
     *  @param x Y position.
     *  @param da_opt drawing area to assign to the image during the operation.
     */ 
    void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL);

    /** Obsolete - will be removed
        @todo remove this method.
     */ 
    void putbox (s_int16 x, s_int16 y, drawing_area * da_opt = NULL);

    /** Obsolete - will be removed
        @todo remove this method.
     */ 
    void putbox_mask (s_int16 x, s_int16 y, drawing_area * da_opt = NULL);

    /** Obsolete - will be removed
        @todo remove this method.
     */  
    void putbox_trans (s_int16 x, s_int16 y, u_int8 a,
                       drawing_area * da_opt = NULL);

    /** Obsolete - will be removed
        @todo remove this method.
     */  
    void putbox_mask_trans (s_int16 x, s_int16 y, u_int8 a,
                            drawing_area * da_opt = NULL);
    
    /** Take a screenshot.
     *  resize the image accordingly to the size of the %screen,
     *  and puts it's content into it.
     */ 
    void screen_shot ();

    /** Get an area from the %screen.
     *  Copies into the image the area of the %screen at position (x, y)
     *  and of size the size of the image.
     *  @param x X position.
     *  @param y Y position.
     */ 
    void get_from_screen (s_int16 x, s_int16 y);

    /** Zoom an image.
     *  Zoom the content of the src image to the size of this image.
     *  @param src the source image to zoom.
     *  @sa resize () 
    */ 
    void zoom (image * src);

    /** Reverse an image horizontally.
     *  Reverse the src image horizontally into this image.
     *  @param src the source image to reverse.
     */ 
    void reverse_lr (image * src);

    /** Reverse an image vertically.
     *  Reverse the src image vertically into this image.
     *  @param src the source image to reverse.
     */ 
    void reverse_ud (image * src);

    /** Apply a "brightness" to an image.
     *  Lighten (or darken) the src image and put the result into this image.
     *  @param src the source image to lighten/darken.
     *  @param cont the "brightness" value, if > 32768 the image will be
     *              lightened, else it will be darkened.
     *  @param proceed_mask if set to true, then the translucent pixels will
     *                      be lightened/darkened too.
     */
    void brightness (image * src, u_int16 cont, bool proceed_mask = false);

    /** Tile an image.
     *  Tile the src image so this image is totally filled.
     *  @param source the source image to tile.
     */ 
    void putbox_tile_img (image * source);

    /** Obsolete - will be replaced
        @todo replace this method.
     */  
    void putbox_img (image * source, u_int16 x, u_int16 y);

    /** Obsolete - will be replaced
        @todo replace this method.
     */  
    void putbox_mask_img (image * source, u_int16 x, u_int16 y);

    /** Obsolete - will be replaced
        @todo replace this method.
     */  
    void putbox_part_img (image * source, u_int16 x, u_int16 y, u_int16 bw,
                          u_int16 bh, u_int16 xo, u_int16 yo);

    /** Get a pixel from the image in %screen format.
     *  Return the pixel at position (x, y) of the image, in %screen format,
     *  suitable for use with put_pix () 
     *  @param x X coordinate of the pixel to return.
     *  @param y Y coordinate of the pixel to return.
     *  @sa put_pix ()
     */ 
    u_int32 get_pix (u_int16 x, u_int16 y);
 
    /** Put a pixel to the image in %screen format.
     *  Put the col %screen format pixel to position (x, y) of the image,
     *  suitable for quickly modifying images which doesn't need to be
     *  saved.
     *  @param x X coordinate where to put the pixel.
     *  @param y Y coordinate where to put the pixel.
     *  @param col pixel in %screen format.
     *  @sa get_pix ()
     */ 
    void put_pix (u_int16 x, u_int16 y, u_int32 col);
 
 private:
#ifdef _EDIT_
    /// Raw image data
    void *simpledata;
#endif
    /// Drawing area where to draw to
    drawing_area *draw_to;

    /// Correctly prepare the blitting operation into the
    /// attached drawing_area.
    inline void get_rects (s_int16 x, s_int16 y);

#ifdef _DEBUG_
    static u_int16 a_d_diff;
#endif
    /// Actual image.
    SDL_Surface *data;

    /// Length and height.
    u_int16 length_, height_;

    /// Bytes per pixel (image depth).
    u_int8 bytes_per_pixel;

    /// Is mask on? 
    bool mask_on;

    /// Alpha value.
    u_int8 alpha_;

    /// Initialise an image.
    void init ();

 public:
#ifdef _EDIT_
    /** Save an image into an opened file, in %game format, with
     *  alpha and mask values.
     *  @param file opened file where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa save ()
     */
    s_int8 put (ogzstream& file);

    /** Save an image into an file, in %game format, with
     *  alpha and mask values.
     *  @param fname file name where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa put ()
     */
    s_int8 save (string fname);

    /** Save an image into an opened file, in %game format, without
     *  alpha and mask values.
     *  @param file opened file where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa save_raw ()
     */
    s_int8 put_raw (ogzstream& file);

    /** Save an image into an file, in %game format, without
     *  alpha and mask values.
     *  @param fname file name where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa put_raw ()
     */
    s_int8 save_raw (string fname);

    /** Save an image into an opened file, in PNM format, without
     *  alpha and mask values.
     *  @param file opened file where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa save_pnm ()
     */
    s_int8 put_pnm (SDL_RWops * file);

    /** Save an image into an file, in PNM format, without
     *  alpha and mask values.
     *  @param fname file name where to save into.
     *  @return
     *          - 0 in case of success.
     *          - -1 in case of error.
     *  @sa put_pnm ()
     */
    s_int8 save_pnm (string fname);

    /** Get a pixel from the image in RGB format.
     *  Return the pixel at position (x, y) of the image, in RGB format,
     *  suitable for use with put_pix_rgb () 
     *  @param x X coordinate of the pixel to return.
     *  @param y Y coordinate of the pixel to return.
     *  @sa put_pix ()
     */ 
    u_int32 get_rgb_pix (u_int16 x, u_int16 y);

    /** Put a pixel to the image in RGB format.
     *  Put the col RGB format pixel to position (x, y) of the image,
     *  suitable for (slowly) modifying images which have to be
     *  saved.
     *  @param x X coordinate where to put the pixel.
     *  @param y Y coordinate where to put the pixel.
     *  @param col pixel in RGB format.
     *  @sa get_pix_rgb ()
     */ 
    void put_rgb_pix (u_int16 x, u_int16 y, u_int32 col);
#endif

#ifndef SWIG
    /// Image copy.
    image & operator = (const image & im);
    friend class data_screen;
#endif
};

#endif
