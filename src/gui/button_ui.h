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
 * @file   button_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the button_ui base class.
 * 
 * 
 */


#ifndef BUTTON_UI_H_
#define BUTTON_UI_H_

#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "fileops.h"
#include "background_ui.h"
#include "button_template.h"
#include "border_ui.h"
#include "button.h"
#include "object_ui.h"

namespace gui
{

  class button_ui : public object_ui
    {
    public:

      /**
       * Constructor 
       * @param the button
       */
      button_ui( button * );
  
  
      /**
       * draw the border
       */
      void draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL);

  
      /**
       * resize the button
       */
      void resize();
  
  
      /**
       * move
       */
      void move();
  
  
      /**
       * set the button_template used
       */
      void set_button (button_template * );
  

      /**
       * Destructor
       */
      ~button_ui();


    protected:
  
      border_ui * pressed_border_;

      border_ui * released_border_;

      background_ui * background_;

      // background_ui  * background_;

      button_template * btempl_;

      button * button_;
    };

};
#endif
