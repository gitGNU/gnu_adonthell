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

#ifndef ATK_BODER_TEMPLATE_H_
#define ATK_BODER_TEMPLATE_H_

#include "fileops.h"
#include "image.h"


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
    void load (igzstream is);  
    
    
    /**
     * write image into the stream
     */
    void save (ogzstream os); 

    
    
    /**
     * get a image
     * @param image number
     * @return image
     */
    image * get (u_int8);

    

    /**
     * destructor :  destroy all image
     */
    ~atk_border_template (); 
    
    /** type enum of border possible */
    typedef enum
        {
            BORDER_TOP = 0, BORDER_BOTTOM, BORDER_LEFT, BORDER_RIGHT,
            CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR
        } border_type;
    
protected :

    
    /**
     * Destroy all images
     */
    void destroy (); 
    
    
    /* a tab with image */
    image * img_[8]; 
    
private : 
    
}; 


#endif
