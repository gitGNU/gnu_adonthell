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
 * @file   border.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the border class.
 * 
 * 
 */


#ifndef BORDER_H_
#define BORDER_H_

#include "border_template.h"
#include "container.h"

namepace gui
{

  class border
    {
      public :


	/**
	 * Constructor
	 * @param the container
	 */
	border (container *);

    
      /**
       * draw the border
       */
      void draw (drawing_area * da = NULL, surface * sf = NULL); 
    
    
      /**
       * resize the border
       */
      void resize (); 
    
    
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
    
    
      protected :
    
    
    
	private :

	/*  container */
	container * container_; 
    
      /* a link on border_template */
      border_template * btempl_; 

      /* image with no brightness */
      image * img_[border_template::_LAST];

      /* image with brightness */
      image * imgbright_[border_template::_LAST];

      /* image use to draw */
      image ** imguse_; 
    
    }; 
};

#endif












