/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   atk_font.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the font TFF class.
 * 
 * 
 */
#ifndef ATK_FONT_H_
#define ATK_FONT_H_

#include <string>  
#include <vector> 

#include <ft2build.h>
#include FT_FREETYPE_H



/**
 * Designed to manipulate font TTF. You can load TTF font, you set up your configuration (size, dpi, color), next call build ()  to apply changement.
 * 
 *
 */ 
class atk_font
{
public : 

    /** Constructor initialize some variables : 
     *     - Set color to black color
     *     - Set size to 12
     *     - Set dpi to 72 
     */ 
    atk_font (); 


    /** Destructor
     *     - Free memory used by font
     */
    ~atk_font (); 
    
     
    /** Load a TTF font file,  before load free memory
     * Important : this method doesn't call build ()  method 
     *
     * @param filename the TTF file
     * @return true : if open successful,  false otherwise
     */
    bool load (const std::string & filename); 

    
    /** Set dots per inch
     * You must set to 72 or 96,  Why ? see documentation on www.freetype.org
     *
     * @param dots per inch
     */ 
    inline void set_dpi (int);

    
    /** Set police size
     *
     * @param font size 
     */
    inline void set_size (int size); 

    
    /** Set color
     *
     * @param r : red color
     * @param g : green color
     * @param b : blue color
     */
    void set_color (u_int8 r, u_int8 g, u_int8 b);


    /** Set color
     *
     * @param col : color 
     */
    void set_color (u_int32 col); 
    
    
    /** Information on the TTF font, write font information in console
     *
     */
    void info ();  
    
 
    /**
     * Build the font with cur configuration (size, color, dpi) 
     * After build, you can draw some text;) 
     */
    bool build ();
    
     

    /** 
     * Draw text at x, y position on target,  if target is NULL draw text on the screen
     * Important,  your font must be build
     *
     * @param text : text to draw
     * @param x : x position
     * @param y : y position
     * @param target : draw on target,  if not target,  draw on the screen
     */
    void draw (const std::string & text, s_int32 x, s_int32 y, surface * target = NULL);  


    
    /**
     * length_of calcul and return length of text
     * Important : if your text is "foo\nfoo" and length of "foo" is 12 px, this method return 24.
     *
     * @param text : text to calcul
     * @return length of the text in pixel
     */
    u_int32 length_of (const std::string & text);
    
    
private : 


    /* a structure which contain all information and image for a char */
    struct char_info
    {
        s_int32 top;

        s_int32 left; 

        s_int32 advance_x;

        image * picture; 
    }; 
    
    
    /* free vector */
    void free_vector (); 

    /* copy bitmap to image */
    void copy_bitmap_to_image (u_int8 * bitmap_data, image * dest); 

    /* close the library and face */
    void close (); 
    
    /* filename of the font */
    std::string filename; 
    
    /* valid font,  true if valid */
    bool valid; 
    
    /* library FT */
    FT_Library library; 
    
    /* FT_Face */
    FT_Face face;

    /* dpi, dots-per-inch */
    int dpi;

    /* size */
    int size;

    /* contain all characters */
    std::vector <char_info> chars;       

    /* color variable */
    u_int8 r, g, b;  
}; 

#endif

