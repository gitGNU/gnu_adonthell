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
 * @file   atk_border_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_border class.
 * 
 * 
 */


#ifndef ATK_BORDER_UI_H_
#define ATK_BORDER_UI_H_

#include "drawing_area.h"
#include "atk_border_template.h"
#include "atk_container.h"
#include "atk_object_ui.h"

class atk_border_ui : public atk_object_ui
{
public :


    /**
     * Constructor
     * @param the container
     */
    atk_border_ui (atk_container *);

    
    /**
     * draw the border
     */
    void draw (drawing_area * da = NULL, surface * sf = NULL); 
    
    
    /**
     * resize the border
     */
    void resize (); 
    

    /**
     * move the border
     */
    void move();

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

    
    /**
     * check if x, y are inside the top right corner
     * @param x : x position
     * @param y : y position
     * @return true if there are inside, false otherwise
     */
    bool is_in_ctr(s_int32 x, s_int32 y);

    
     /**
     * check if x, y are inside the top left corner
     * @param x : x position
     * @param y : y position
     * @return true if there are inside, false otherwise
     */
    bool is_in_ctl(s_int32 x, s_int32 y); 
    

    /**
     * check if x, y are inside the bottom right corner
     * @param x : x position
     * @param y : y position
     * @return true if there are inside, false otherwise
     */
    bool is_in_cbr(s_int32 x, s_int32 y); 
    

    /**
     * check if x, y are inside the top border
     * @param x : x position
     * @param y : y position
     * @return true if there are inside, false otherwise
     */
    bool is_in_bt(s_int32 x, s_int32 y); 
    

    /**
     * Destructor
     */
    ~atk_border_ui();
    
    
    protected :
      
        
        
    private :
        
      /**
       * Alloc memory for each image
       */
      void alloc ();
    
    
    /* atk container */
    atk_container * container_; 
    
    /* a link on border_template */
    atk_border_template * btempl_; 

    /* image with no brightness */
    drawing_area * img_[atk_border_template::_LAST];
}; 

#endif












