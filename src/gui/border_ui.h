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
 * @file   border_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the border class.
 * 
 * 
 */


#ifndef BORDER_UI_H_
#define BORDER_UI_H_

#include "gfx/drawing_area.h"
#include "border_template.h"
#include "container.h"
#include "object_ui.h"

namespace gui
{

  class border_ui : public object_ui
    {
      public :


	/**
	 * Constructor
	 * @param the container
	 */
	border_ui (container *);

    
      /**
       * draw the border
       */
      void draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
    
    
      /**
       * resize the border
       */
      void resize (); 
    

      /**
       * move the border
       */
      void move();

      /**
       * set the border_template,  when this object is delete,  it doesn't delete border_template
       */
      void set_border (border_template *); 

    
      /**
       * clear this object,  free memory,  set border to NULL
       */
      void clear (); 


      /**
       * return template border
       */
      border_template * get_template () const; 

    
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
      ~border_ui();
    
    
      protected :
      
        
        
	private :
        
	/**
	 * Alloc memory for each image
	 */
	void alloc ();
    
    
      /* container */
      container * container_; 
    
      /* a link on border_template */
      border_template * btempl_; 

      /* image with no brightness */
      gfx::drawing_area * img_[border_template::_LAST];
    }; 
}
#endif












