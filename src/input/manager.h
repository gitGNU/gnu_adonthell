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
 * @file   manager.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the manager static class.
 * 
 * 
 */


#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "listener.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "joystick_event.h"
#include "control_event.h"
#include <list> 

namespace input
{

    /**
     * This class is responsible for handling all input events and
     * raise callback methods accordingly. It mainly handles a list
     * of input_listeners which hears to certain types of events. When
     * an input event occurs, this class checks for input_listeners
     * that listens to this kind of event and call their callback
     * function with the event as parameter.
     * 
     */
    class manager
    {
    public:
    
        /** 
         * Initialisation method. This function must be called
         * before any use of this class.
         * 
         */
        static void init();

        /**
         * Cleanup method. Call this function when you don't need
         * the input manager any more, to cleanup it's ressources.
         * 
         */
        static void cleanup();

        /**
         * Update the manager status. This method should be 
         * called once per game cycle. It will reads all the events
         * in the event queues and call the connected callback methods 
         * accordingly.
         * 
         */
        static void update();

        /**
         * Raise an arbitrary event. This will simulate the event and
         * will behave as if it really occured.
         * 
         */
        static void raise_event(event & ev);

        /**
         * Add an input_listener to the list of handled listeners.
         * 
         */
        static void add(listener * il)
        {
            listeners.push_back(il);
        }
    
        /**
         * Remove an input_listener from the list of handled listeners.
         * 
         */
        static void remove(listener * il)
        {
            listeners.remove(il);
        }

    private:
        static std::list <listener *> listeners;

    }; 
}

#endif
