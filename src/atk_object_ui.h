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
 * @file   atk_object_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_border class.
 * 
 * 
 */


#ifndef ATK_OBJECT_UI_H_
#define ATK_OBJECT_UI_H_

#include "surface.h"
#include "drawing_area.h"

class atk_object_ui
{
 public:

  /**
   * draw the border
   */
  virtual void draw (drawing_area * da = NULL, surface * sf = NULL) = 0;

  
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
  virtual ~atk_object_ui(){};
};
#endif
