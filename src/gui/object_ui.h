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
 * @file   object_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the border class.
 * 
 * 
 */


#ifndef OBJECT_UI_H_
#define OBJECT_UI_H_

#include "gfx/surface.h"
#include "gfx/drawing_area.h"

namespace gui
{

  class object_ui
    {
    public:

      /**
       * draw the border
       */
      virtual void draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL) = 0;

  
      /**
       * resize the border
       */
      virtual void resize () = 0; 
  
  
      /**
       * move the border
       */
      virtual void move() = 0;

      /**
       * Destructor
       */
      virtual ~object_ui(){};
    };

};
#endif
