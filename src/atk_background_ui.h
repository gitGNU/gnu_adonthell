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
 * @file   atk_background_ui.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_border class.
 * 
 * 
 */


#ifndef ATK_BACKGROUND_UI_H_
#define ATK_BACKGROUND_UI_H_

#include "atk_container.h"
#include "atk_object_ui.h"
#include "atk_background_template.h"

class atk_background_ui : public atk_object_ui
{
 public:
  /**
   * Constructor
   * @param the container
   */
  atk_background_ui (atk_container *);
  
  
  /**
   * draw the background
   */
  void draw (drawing_area * da = NULL, surface * sf = NULL); 
  
  
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
  void set_background( atk_background_template * btmp);
  
  
 private:
  
  /**
   * 
   */
  atk_container * container_;
  
  /*
   */
  atk_background_template * btmpl_;
};

#endif


