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

protected:
    u_int8 type;
    char *file;

public:
    virtual void execute (event*) = 0;
    virtual bool equals (event*) = 0;
};

// To notify when a character entered a maptile
class enter_event : public event
{
public:
    enter_event ();

    s_int32 x;
    s_int32 y;
    s_int8 dir;
    s_int32 map;
    character *c;

    void execute (event*);
    bool equals (event*);
}; 

class event_handler
{
public:
    static void register_event (event*, const char*);
    static void raise_event (event*);

private:
    static vector<event*> handlers[MAX_EVENT];
};

#endif // __EVENT__H_