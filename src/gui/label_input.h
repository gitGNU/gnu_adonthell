/*
   
   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef LABEL_INPUT_H_
#define LABEL_INPUT_H_

#include "label.h"

namespace gui {
  
  class label_input : public gui::label
    {
    public:
      /**
	 Constructor
	 Initialize to : cursor_moveable,  cursor_visible and editable
      */ 
      label_input (); 
      
      /**
	 Set the label input in editable
      */
      void set_editable (const bool); 
      
      /**
       Input update
      */
      bool input_update();
      
    protected:
      bool editable_; 
    };
}

#endif



