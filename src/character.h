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

#include <stdio.h>

struct PyCodeObject;

enum
{
    DWARF = 0,
    ELF = 1,
    HALFELF = 2,
    HUMAN = 3
};

enum
{
    FEMALE = 0,
    MALE = 1
};

// Base class for all sort of characters, like the player, NPC's, etc.
#ifdef SWIG
class character : public storage
#else
class character : public storage, public event_list
#endif // SWIG
{
public:
    char *name;                             // The character's name (and ID)
    u_int16 posx;                           // The x position on the (current?) map
    u_int16 posy;                           // The y position
    u_int32 color;                          // Color of dialogue text

#ifndef SWIG
    void save (FILE*);                      // Save the character to file
    void load (FILE*, bool b=true);         // Load the character from file
#endif
};

// Representation of a NPC
class npc : public character
{
public:
    npc ();                                 // Constructor
    ~npc ();                                // Destructor
    
    void set_schedule (char*, bool b=true); // Set / change the active schedule
    void set_dialogue (char*);              // Set / change the active dialogue

    u_int8 move (u_int8);                   // Executes the schedule script

#ifndef SWIG
    char* get_dialogue ();                  // Returns the character's dialogue
    void save (FILE*);                      // Save the character to file
    void load (FILE*, bool b=true);         // Load the character from file

protected:
    char* dialogue;                         // The NPC's dialogue
    char* schedule_file;                    // The schedule file
    PyCodeObject *schedule;                 // The NPC's "behaviour" script
#endif
};

// The Player's main character
class player : public character
{
public:
protected:
};

#endif // __CHARACTER_H__
