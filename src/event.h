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
#include <zlib.h>

#include "Python.h"
#include "compile.h"
#include "types.h"

class event_handler;
class character;

enum
{
    ENTER_EVENT = 0,                            // Characters reach a new tile
    LEAVE_EVENT = 1,                            // Characters leave a tile
    TIME_EVENT = 2,                             // A minute of gametime passed
    MAX_EVENT = 3
};

// Baseclass for event data
class event
{
friend event_handler;

public:
    char* script_file;                          // Filename of the event script
    virtual void save (gzFile) = 0;             // save the event data
    virtual ~event ();
    
// Don't grant direct access to these: only event_handler may set/modify
// event type or script (for safety reasons)
protected:
    u_int8 type;                                // event type -> see enum above
    PyCodeObject *script;                       // precompiled script

    virtual void execute (event*) = 0;          // execute the script
    virtual bool equals (event*) = 0;           // compare two events for equality
    virtual void load (gzFile) = 0;             // load the event data
};

// Baseclass for enter/leave events
class base_map_event : public event
{
public:
    void save (gzFile);                         // Save event data

    s_int32 x;                                  // x coordinate
    s_int32 y;                                  // y coordinate
    s_int8 dir;                                 // walking direction
    s_int32 map;                                // map
    character *c;                               // character triggering the event

protected:
    base_map_event ();

    void execute (event*);                      // Run the event's script
    bool equals (event*);                       // Compare two events
    void load (gzFile);                         // Load event data
};

// To notify when a character entered a maptile
class enter_event : public base_map_event
{
public:
    enter_event ();
};

// To notify when a character entered a maptile
class leave_event : public base_map_event
{
public:
    leave_event ();
};

// To notify at a certain time
class time_event : public event
{
public:
    time_event ();
    void save (gzFile);                         // Save event data

    u_int8 minute;                              // 0 - 59
    u_int8 m_step;                              // 0, 1, 2, ...
    u_int8 hour;                                // 0 - 23
    u_int8 h_step;                              // 0, 1, 2, ...
    u_int8 day;                                 // 0 - 27
    u_int8 d_step;                              // 0, 1, 2, ...
    u_int32 time;                               // the actual gametime in minutes

protected:
    void execute (event*);                      // Run the event's script
    bool equals (event*);                       // Compare two events
    void load (gzFile);                         // Load event data
};

// Base class for objects that want to register events
class event_list
{
public:
    vector<event*> events;                      // List of registered events
    ~event_list ();                             // Unregister all events
};

// Keeps track of registered scripts, recieves triggered events 
// and executes scripts handling those events
class event_handler
{
public:
    static void register_event (event*, char*); // register an event
    static void remove_event (event*);          // unregister an event
    static void raise_event (event*);           // event triggered
    static event* load_event (gzFile, bool=true);// load an event
    
private:
    static vector<event*> handlers[MAX_EVENT];  // registered events storage
};

#endif // __EVENT__H_