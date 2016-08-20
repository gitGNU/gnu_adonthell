/*
   Copyright (C) 1999/2000/2001   Alexandre Courbot
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


/**
 * @file   drawable.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief Defines the drawable class. 
 */


#include "drawable.h"
 

// Public methods.


drawable::drawable () 
{
    set_length (0);
    set_height (0); 
}

drawable::~drawable ()
{ 
}

bool drawable::update ()
{
    return true; 
}

bool drawable::input_update ()
{
    return true; 
}
