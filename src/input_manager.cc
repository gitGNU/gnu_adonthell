/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   input_manager.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com
 * 
 * @brief  Defines the input_manager class members that are
 *         system-independant.
 * 
 * 
 */


#include "input_manager.h"

void input_manager::raise_event(input_event & ev)
{
    for (list<input_listener *>::iterator it = listeners.begin(); 
         it != listeners.end(); it++)
        if ((*it)->is_listening_to(ev.type()))
            (*it)->raise_event(&ev);
}
