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
 * @file   button_template.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the widget base class.
 * 
 * 
 */


#ifndef BUTTON_TEMPLATE_H_
#define BUTTON_TEMPLATE_H_

#include <string>
#include "fileops.h"
#include "object_template.h"
#include "border_template.h"
#include "background_template.h"

namespace gui
{

  class button_template: public object_template
    {
    public:
  
      /**
       * Constructor
       */
      button_template();
  
      /**
       * read a stream to load image
       */
      void load (igzstream & is);
  
  
      /**
       * write image into the stream
       */
      void save (ogzstream & os); 
  
  
      /**
       * set border on released
       */
      void set_released (border_template *);

      /**
       * set border on pressed
       */
      void set_pressed (border_template *);


      /**
       * set background
       */
      void set_background (background_template *);


      /**
       * @return pressed border
       */
      border_template * get_pressed ()const ;
  

      /**
       * @return released border
       */
      border_template * get_released () const;
  
  
      /**
       * @return the background
       */
      background_template * get_background() const;

      /**
       * display some information
       */
      void display_info ();

      /**
       * build nothing ...
       */
      void build ();


      /**
       * Destructor
       */
      ~button_template();

    protected:


      background_template * background_;
  
      border_template * pressed_border_;
  
      border_template * released_border_;
  
    };

};
#endif
