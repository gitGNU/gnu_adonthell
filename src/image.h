/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   image.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the image class.
 * 
 * 
 */


#ifndef IMAGE_H_
#define IMAGE_H_

#include "fileops.h"
#include "screen.h"


/**
 * Image manipulation class.
 * Designed to work with single images, without having to care about the bit
 * depth. This class is widely used through the %game - in fact it handles
 * everything that is displayed on the %screen.
 * This class highly relies on surface, so you'll probably want to have a look
 * at it before using image.
 */
class image : public surface
{
public:
    /**
     * Default constructor.
     * The image created via this constructor is totally empty.
     */ 
    image ();
    
#ifndef SWIG
    /** Creates an image with a specified size.
     *  @param l length of the image.
     *  @param h height of the image.
     *
     *  @attention Not accessible from Python.
     */ 
    image (u_int16 l, u_int16 h);
#endif
    
    /** Destructor
     */ 
    ~image (); 

    /** 
     * Resize this image.
     * All the content will be lost.
     * If you want to zoom the image you'll want to see the zoom () function
     * instead.
     *
     * @param l new length.
     * @param h new height.
     * @sa zoom () 
     */
    void resize (u_int16 l, u_int16 h); 
    
    /** 
     * Resets the image to it's initial state, that is totally
     * empty.
     * 
     */
    void clear (); 
    

    /**
     * @name Loading / Saving Methods.
     * These methods allows you to load and save
     * an image in different formats.
     * 
     */ 
    //@{ 
    

    /** Loads an image from an opened file, saved in %game internal format,
     *  with alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa load ()
     */ 
    s_int8 get (igzstream& file);
    
    /** Loads an image from a file name, in game internal format, with alpha
     *  and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa get ()
     */
    s_int8 load (string fname);

    /** Loads an image from an opened file, saved in %game internal format,
     *  without alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa load_raw () 
     */
    s_int8 get_raw (igzstream& file);
    
    /** Loads an image from a file name, in game internal format, without alpha
     *  and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa get_raw ()
     */ 
    s_int8 load_raw (string fname);

    /** Loads an image from an opened file, in PNM format, without
     *  alpha and mask values.
     *  @param file the opened file from which to read.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa load_pnm () 
    */
    s_int8 get_pnm (SDL_RWops * file);
    
    /** Loads an image from a file name, in PNM format, without
     *  alpha and mask values.
     *  @param fname the name of the file to load.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa get_pnm ()
     */ 
    s_int8 load_pnm (string fname); 
    
    /** Saves an image into an opened file, in %game format, with
     *  alpha and mask values. 
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save ()
     */
    s_int8 put (ogzstream& file);

    /** Saves an image into an file, in %game format, with
     *  alpha and mask values.
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put ()
     */
    s_int8 save (string fname);

    /** Saves an image into an opened file, in %game format, without
     *  alpha and mask values.
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save_raw ()
     */
    s_int8 put_raw (ogzstream& file);

    /** Saves an image into an file, in %game format, without
     *  alpha and mask values.
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put_raw ()
     */
    s_int8 save_raw (string fname);

    /** Saves an image into an opened file, in PNM format, without
     *  alpha and mask values.
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param file opened file where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa save_pnm ()
     */
    s_int8 put_pnm (SDL_RWops * file);

    /** Saves an image into an file, in PNM format, without
     *  alpha and mask values.
     *  @warning as the image which is saved comes from a %screen's depth
     *           surface, it will be slightly altered during the save.
     *           If you want a class capable of saving images with full
     *           truecolor quality, use image_edit instead.
     *  @param fname file name where to save into.
     *  @return
     *      @li 0 in case of success.
     *      @li -1 in case of error.
     *  @sa put_pnm ()
     */
    s_int8 save_pnm (string fname);


    //@}
    

    /**
     * @name Special FX Methods.
     * Allows you to put fantasy in your image manipulations! Can
     * eventually even be usefull...
     * 
     */ 
    //@{

    
    /** Zooms a surface.
     *  Zoom the content of the src surface into this image, to it's own size.
     *  @param src the source image to zoom.
     */
    void zoom (surface& src)
    {
        zoom (src, length (), height (), 0, 0);
    }
    
#ifndef SWIG
    /** 
     * Zooms a surface.
     * Zoom the content of the src surface into this image, to the size
     * (l, h), at position (x, y) on this image.
     * 
     * @param src The source surface to zoom.
     * @param l length of the zoomed image.
     * @param h height of the zoomed image.
     * @param x X offset on the destination image.
     * @param y Y offset on the destination image.
     *
     * @attention Not available from Python. Use zoom_to () from Python instead.
     * @sa zoom_to () 
     */
    void zoom (surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0);
#endif
    
    /**
     * Synonym of zoom () to guarantee its access from Python.
     *
     * @sa zoom () 
     * 
     */ 
    void zoom_to (surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0)
    {
        zoom (src, l, h, x, y); 
    }
    
    /** Tiles a surface.
     *  Tiles the src surface so this image is totally filled.
     *  @param source the source surface to tile.
     */ 
    void tile (surface& src)
    {
        tile (src, length (), height ()); 
    }

#ifndef SWIG
    /**
     * Tiles a surface.
     * Tiles the src surface so the area of this image starting at position
     * (x, y) and (l, h) sized is totally filled.
     * @param source the source surface to tile.
     * @param l length of the area to tile.
     * @param h height of the area to tile.
     * @param x X offset on the destination image.
     * @param y Y offset on the destination image.
     *
     * @attention Not available from Python. Use tile_to () from Python instead.
     * @sa tile_to () 
     */ 
    void tile (surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0);
#endif
    
    /**
     * Synonym of tile () to guarantee its access from Python.
     *
     * @sa tile () 
     * 
     */ 
    void tile_to (surface& src, u_int16 l, u_int16 h, u_int16 x = 0, u_int16 y = 0)
    {
        tile (src, l, h, x, y); 
    }

    /**
     * Applies a "brightness" to a surace.
     * Lighten (or darken) the src surface and put the result into this image.
     * This image will be resized to the src surface's size. 
     * @param src the source surface to lighten/darken.
     * @param cont the "brightness" value, if < 256 the image will be
     *             darkened.
     * @todo modify it so when < 128 -> darken, > 128 -> brighten.
     * @param proceed_mask if set to true, then the translucent pixels will
     *                     be lightened/darkened too.
     */
    void brightness (surface& src, u_int8 cont, bool proceed_mask = false); 
 
    //@} 


#ifndef SWIG
    /**
     * Image copy (similar to copy ()).
     *
     * @attention Not available from Python. Use copy () from Python instead.
     * @sa copy ()
     */
    image& operator = (image& src); 
#endif

    /**
     * Synonym of operator = to guarantee its access from Python.
     *
     * @sa operator = 
     */
    void copy (image& src) 
    {
        *this = src; 
    }
 
private:
    /**
     * Copy-constructor (forbids value passing).
     */
    image(image& src);
    
    /** 
     * Converts a raw image source recorded in RGB to the current screen depth
     * and put it to this image.
     * 
     * @param rawdata raw data to convert.
     * @param l length of the raw image.
     * @param h height of the raw image.
     */
    void raw2display (void * rawdata, u_int16 l, u_int16 h); 
}; 


#endif
