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
 * @file   atk_border_template.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_border_template class.
 * 
 * 
 */

#ifndef ATK_BORDER_TEMPLATE_H_
#define ATK_BORDER_TEMPLATE_H_

#include "fileops.h"
#include "gfx/image.h"


class atk_border_template
{
    public : 
        
        /**
         * Default constructor,  set all images to null
         */
        atk_border_template (); 
    
    
    /**
     * read a stream to load image
     */
    void load (igzstream & is);  
    
    
    /**
     * write image into the stream
     */
    void save (ogzstream & os); 
    
    
    
    /**
     * get name of the border
     * @return name of the border
     */
    std::string get_name () const;
    
    
    /**
     * set name of the border
     * @param name :  name of the border
     */
    void set_name (const std::string & name); 
    

    /**
     * Build is automaticly call when you load a border
     * build create 4 big border
     */
    void build();

    /**
     * get a image
     * @param image number
     * @return image
     */
    gfx::image * get (u_int8);


    /**
     * get a border image
     * @param the image number {B_TOP,B_BOTTOM, B_LEFT, B_RIGHT }
     */
    gfx::image * get_border(u_int8);

    /**
     * set a image
     * @param type of the image (corner,  border ...)
     * @param img :  the image
     */
    void set (u_int8 type, gfx::image * img);  

    
    /**
     * set a image
     * @param type of the image (corner,  border ...)
     * @param filename : filename of the image (filename must be only a pnm file) 
     */
    void set (u_int8 type, std::string filename);  


    /**
     * write in standart ouput border information 
     */
    void display_info () const;
    

    /**
     * destructor :  destroy all image
     */
    ~atk_border_template (); 
    
    /** type enum of border possible */
    typedef enum
        {
            B_TOP = 0, B_BOTTOM, B_LEFT, B_RIGHT,
            C_TL, C_TR, C_BL, C_BR, C_EXIT,  C_RESIZE, _LAST
        } border_type;

   
    protected :
        
        /**
         * Destroy all images
         */
    void destroy (); 
 
 
    /* a tab with image */
    gfx::image * img_[_LAST]; 


    /* this tab is used to store big border, it is necessary to optimise  resize and to reduce memory use */
    gfx::image * imgborder_[C_TL];

    /* name of the border */
    std::string name_; 

private : 
    
}; 


#endif




