/*
   $Id$

   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>
#include "character.h"
#include "types.h"

struct PyCodeObject;

enum
{
    ENTER_EVENT = 0,
    MAX_EVENT = 1
};

class event_handler;

// Baseclass for event data
class event
{
friend event_handler;

// Don't grant direct access to these: only event_handler may set/modify
// event type or script (for safety reasons)
protected:
    u_int8 type;                        // event type -> see enum above
    PyCodeObject *script;               // precompiled script

    virtual void execute (event*) = 0;  // execute the script
    virtual bool equals (event*) = 0;   // compare two events for equality
};

// To notify when a character entered a maptile
class enter_event : public event
{
public:
    enter_event ();

    s_int32 x;                          // x coordinate
    s_int32 y;                          // y coordinate
    s_int8 dir;                         // walking direction
    s_int32 map;                        // map
    character *c;                       // character triggering the event

    void execute (event*);
    bool equals (event*);
}; 

class event_handler
{
public:
    static void register_event (event*, char*); // register an event
    static void raise_event (event*);           // event triggered

private:
    static vector<event*> handlers[MAX_EVENT];  // registered events storage
};

#endif // __EVENT__H_