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
 * @file   manager.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com
 * 
 * @brief  Defines the manager class members that are
 *         system-independant.
 * 
 * 
 */


#include "manager.h"

using namespace input;

void manager::raise_event(event & ev)
{
    for (std::list<listener *>::iterator it = listeners.begin(); 
         it != listeners.end(); it++)
        if ((*it)->is_listening_to(ev.type()))
            (*it)->raise_event(&ev);
}
