/*
   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LABEL_INPUT_H_
#define LABEL_INPUT_H_

#include "label.h"


class label_input : public label
{
 public:
    /**
       Constructor
       Initialize to : cursor_moveable,  cursor_visible and editable
    */ 
    label_input (); 

    virtual ~label_input ();

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


#endif



