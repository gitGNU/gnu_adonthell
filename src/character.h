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

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "types.h"
#include "storage.h"
#include "inventory.h"
#include "event.h"
#include <vector>

struct PyCodeObject;

// Base class for all sort of characters, like the player, NPC's, etc.
#ifdef SWIG
class character : public storage
#else
class character : public storage, public event_list
#endif // SWIG
{
public:
    char *name;                     // The character's name (and ID)
    u_int16 posx;                   // The x position on the (current?) map
    u_int16 posy;                   // The y position

protected:
    s_int32 type;                   // The characters type (needed at all?)
};

// Representation of a NPC
class npc : public character
{
public:
    npc ();                         // Constructor

    void set_schedule (char*);      // Set / change the active schedule
    void set_dialogue (u_int32);    // Set / change the active dialogue
    const char* talk ();            // Returns the active dialogue
    u_int8 move (u_int8);           // Run the active schedule

protected:
    vector<char*> dialogues;        // All the dialogues available to that NPC
    u_int32 active_dialogue;        // Dialogue to initiate when speaking to the NPC
    PyCodeObject *schedule;         // The NPC's "behaviour" script
};

// The Player's main character
class player : public character
{
public:
protected:
};

#endif // __CHARACTER_H__
