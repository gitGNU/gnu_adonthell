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
 * @file   atk_border.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_border class.
 * 
 * 
 */


#ifndef ATK_BORDER_H_
#define ATK_BORDER_H_

#include "atk_border_template.h"
#include "atk_container.h"

class atk_border
{
public :


    /**
     * Constructor
     * @param the container
     */
    atk_border (atk_container *);

    
    /**
     * draw the border
     */
    void draw (drawing_area * da = NULL, surface * sf = NULL); 
    
    
    /**
     * resize the border
     */
    void resize (); 
    
    
    /**
     * set the atk_border_template,  when this object is delete,  it doesn't delete atk_border_template
     */
    void set_border (atk_border_template *); 


    /**
     * clear this object,  free memory,  set border to NULL
     */
    void clear (); 


    /**
     * return template border
     */
    atk_border_template * get_template () const; 
    
    
protected :
    
    
    
private :

    /* atk container */
    atk_container * container_; 
    
    /* a link on border_template */
    atk_border_template * btempl_; 

    /* image with no brightness */
    image * img_[atk_border_template::_LAST];

    /* image with brightness */
    image * imgbright_[atk_border_template::_LAST];

    /* image use to draw */
    image ** imguse_; 
    
}; 

#endif












