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
#include <vector>


// Base class for all sort of characters, like the player, NPC's, etc.
class character : public storage
{
public:
    void set_name (const char*);
    char *get_name ();
    
protected:
    s_int32 type;                   // The characters type
    char *name;                     // The character's name (and ID)
};

// Representation of a NPC
class npc : public character
{
public:
    void set_dialogue (u_int32);    // Set / change the active dialogue
    const char* talk ();            // Returns the active dialogue
    
protected:
    vector<char*> dialogues;        // All the dialogues available to that NPC
    u_int32 active_dialogue;        // Dialogue to initiate when speaking to the NPC
};

// The Player's main character
class player : public character
{
public:
protected:
};

#endif // __CHARACTER_H__