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
 * @file   background_template.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the background_template class.
 * 
 * 
 */

#ifndef BACKGROUND_TEMPLATE_H_
#define BACKGROUND_TEMPLATE_H_

#include "fileops.h"
#include "gfx/image.h"

namespace gui
{

  
  class background_template
    {
    public:
      /**
       * Constructor
       */
      background_template();
      
  
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
       * @return the background image
       */
      gfx::image * get_image();
      
      
      /**
       * @return the background color
       */
      u_int8 get_r();
      
      /**
       * @return the background color
       */
      u_int8 get_g();
      
      /**
       * @return the background color
       */
      u_int8 get_b();
      
      /**
       * Set the image for the background
       */
      void set_image (gfx::image * tmp);
      
      
      /**
       * set color of the background
       */
      void set_color (u_int8 r, u_int8 g, u_int8 b);
      
      
      /**
       * @return background type
       */
      u_int8 get_type();
      
      
      typedef enum { NOTHING, COLOR, IMAGE } background_type;
      
    protected:
      
      gfx::image * imgback_;
      
      //u_int32 colorback_;
      
      u_int8 r_;
      u_int8 g_;
      u_int8 b_;
      
      std::string name_;
      
      u_int8 type_;
    };

};
#endif
