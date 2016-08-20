/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file map_event.h
 *
 * @author Kai Sterker
 * @brief Declares the different map events.
 */

#ifndef MAP_EVENT_H__
#define MAP_EVENT_H__

#include "event.h"
#include "character.h"

/**
 * Baseclass for map enter/leave/action events.
 * The event will be launched if all the member's data matches.
 */ 
class map_event : public event
{
public:
    /**
     * Default constructor.
     */ 
    map_event ();

    /** 
     * Saves the basic %event %data (such as the type or script data)
     * to a file.
     * 
     * @param out file where to save the %event.
     */ 
    void put_state (ogzstream&) const;

    /** 
     * Loads the basic %event %date from a file.
     * 
     * @param in file to load the %event from.
     * @return \e true if the %event could be loaded, \e false otherwise
     */
    bool get_state (igzstream&);

    /**
     * Submap index (-1 for any).
     */
    s_int32 submap;

    /**
     * X position (-1 for any).
     */
    s_int32 x;

    /**
     * Y position (-1 for any).
     */
    s_int32 y;

    /**
     * Direction where the character is looking (-1 for any).
     */ 
    s_int8 dir;

    /**
     * Useless (for now).
     */ 
    s_int32 map;

    /**
     * Pointer to the mapcharacter that can launch this event (NULL for any).
     */ 
    mapcharacter *c;

    /**
     * Compare two map events for equality.
     *
     * @param evnt The map event to compare this to.
     * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
     */
    bool equals (const event* evnt);

    /**
     * Executes the script associated with this map %event. If the
     * event does not repeat it is removed from the %event handler.
     *
     * @param evnt The %event that triggered this map %event.
     *
     * @return The number of times the %event needs to be repeated.
     */
    s_int32 execute (const event* evnt);
};


/**
 * To notify when a character entered a mapsquare.
 */ 
class enter_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    enter_event ();
};


/**
 * To notify when a mapcharacter left a mapsquare.
 */ 
class leave_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    leave_event ();
};


/**
 * To notify when a mapcharacter "act" on a square.
 */ 
class action_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    action_event ();
};

#endif // MAP_EVENT_H__
