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
 * @file   atk_button.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_button base class.
 * 
 * 
 */

#ifndef ATK_BUTTON_H_
#define ATK_BUTTON_H_
#include <string>

#include "input/event.h"
#include "gfx/surface.h"
#include "gfx/drawing_area.h"
#include "atk_font.h"
#include "atk_bin.h"
#include "atk_button_template.h"

#include "callback_sig.h"


class atk_button : public atk_bin
{
 public:
  
  /**
   * Constructor
   */
  atk_button ();
  
  
  /**
   * @return true if the button is pressed, false otherwise
   */
  bool get_pressed() const;
  
  
  /**
   * Define the button 
   */
  void set_button_ui (atk_button_template * but);

  
  /**
   * Set text inside the button
   */
  void set_text (const std::string & text, atk_font * font);
    
  
  /**
   * input update function
   * @return 1 if this object use the event,  else return 0
   */
  virtual int input_update (input::event *); 
  
  
  /**
   * Constructor
   */
  virtual ~atk_button();
  
  
  /**
   * Define all evenement
   */
  callback_sig on_pressed;
  callback_sig on_released;
  callback_sig on_clicked;
   
 protected:
  
  bool pressed_;
  
 private:
  
};

#endif
