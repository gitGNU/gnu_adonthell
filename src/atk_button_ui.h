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
 * @file   atk_button_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_button_ui base class.
 * 
 * 
 */


#ifndef ATK_BUTTON_UI_H_
#define ATK_BUTTON_UI_H_

#include "gfx/drawing_area.h"
#include "gfx/surface.h"
#include "fileops.h"
#include "atk_background_ui.h"
#include "atk_button_template.h"
#include "atk_border_ui.h"
#include "atk_button.h"
#include "atk_object_ui.h"

class atk_button_ui : public atk_object_ui
{
 public:

  /**
   * Constructor 
   * @param the atk_button
   */
  atk_button_ui( atk_button * );
  
  
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
   * set the atk_button_template used
   */
  void set_button (atk_button_template * );
  

  /**
   * Destructor
   */
  ~atk_button_ui();


 protected:
  
  atk_border_ui * pressed_border_;

  atk_border_ui * released_border_;

  atk_background_ui * background_;

  // atk_background_ui  * background_;

  atk_button_template * btempl_;

  atk_button * button_;
};
#endif
