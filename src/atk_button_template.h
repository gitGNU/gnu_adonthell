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
 * @file   atk_button_template.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_widget base class.
 * 
 * 
 */


#ifndef ATK_BUTTON_TEMPLATE_H_
#define ATK_BUTTON_TEMPLATE_H_

#include <string>
#include "fileops.h"
#include "atk_border_template.h"
#include "atk_background_template.h"


class atk_button_template
{
 public:
  
  /**
   * Constructor
   */
  atk_button_template();
  
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
   * set border on released
   */
  void set_released (atk_border_template *);

  /**
   * set border on pressed
   */
  void set_pressed (atk_border_template *);


  /**
   * set background
   */
  void set_background (atk_background_template *);


  /**
   * @return pressed border
   */
  atk_border_template * get_pressed ()const ;
  

  /**
   * @return released border
   */
  atk_border_template * get_released () const;
  
  
  /**
   * @return the background
   */
  atk_background_template * get_background() const;

  /**
   * Destructor
   */
  ~atk_button_template();

 protected:


  atk_background_template * background_;
  
  atk_border_template * pressed_border_;
  
  atk_border_template * released_border_;
  
  std::string name_;
};
#endif
