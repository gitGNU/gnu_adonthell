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
 * @file   background_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the border class.
 * 
 * 
 */


#ifndef BACKGROUND_UI_H_
#define BACKGROUND_UI_H_

#include "container.h"
#include "object_ui.h"
#include "background_template.h"

namespace gui
{
  
  class background_ui : public object_ui
    {
    public:
      /**
       * Constructor
       * @param the container
       */
      background_ui (container *);
      
      
      /**
       * draw the background
       */
      void draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
      
      
      /**
       * resize the background
       */
      void resize (); 
      
      
      /**
       * move the background
       */
      void move();
      
      
      /**
       * set the background template
       */
      void set_background( background_template * btmp);
      
      
    private:
      
      /**
       * 
       */
      container * container_;
      
      /*
       */
      background_template * btmpl_;
    };
};
#endif


