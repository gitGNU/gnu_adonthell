/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   pnm.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the pnm static class.
 * 
 * 
 */

#include "types.h"

/**
 * Allow reading and writing PNM files.
 * 
 */
class pnm
{
public:
    /** 
     * Reads a PNM %image from an opened file.
     *
     * @warning
     * The returned pointer is allocated by this function with calloc ().
     * Don't forget to free () it when you don't need it anymore!
     * 
     * @param file opened file from which to read.
     * @param length pointer to the integer which will contain the %image's length.
     * @param height pointer to the integer which will contain the %image's height.
     * 
     * @return allocated pointer containing the PNM %image.
     */
    static void *get (SDL_RWops * file, u_int16 * length, u_int16 * height);

    /** 
     * Saves a PNM %image into an opened file.
     * 
     * @param file the opened file to write the PNM %image to.
     * @param image the PNM %image data.
     * @param length the length of the %image to write.
     * @param height the height of the %image to write.
     */
    static void put (SDL_RWops * file, void *image, u_int16 length, u_int16 height);

private:
    
    /// Go to the next file's line.
    static void pnm_gotonextline (SDL_RWops * file);

    /// Skip PNM comments.
    static int pnm_checkforcomment (SDL_RWops * file);
}; 
