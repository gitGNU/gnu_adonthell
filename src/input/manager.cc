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
#include <algorithm>

using namespace input;

void manager::raise_event(event & ev)
{
    for (std::list<listener *>::iterator it = listeners.begin(); 
         it != listeners.end(); it++)
        if ((*it)->is_listening_to(ev.type()))
        {
            int res = (*it)->raise_event(&ev);
            if (res) break;
        }
}

bool manager::give_focus(listener * l)
{
    std::list<listener *>::iterator it = std::find(listeners.begin(), listeners.end(), l);
    if (it != listeners.end())
    {
        listeners.erase(it);
        listeners.push_front(l);
        return true;
    }
    return false;
}
