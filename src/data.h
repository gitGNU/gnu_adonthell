/*
   $Id$

   Copyright (C) 2001 by Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __DATA_H__
#define __DATA_H__

#include "storage.h"
#include "gametime.h"
#include "Python.h"

class data
{
public:
    static bool load (const char*);         // Load a game
    static void save (const char*);         // Save the game

    static PyObject *globals;               // Global namespace to use in scripts
    static gametime *time;                  // The gametime object
    static objects characters;              // All the characters 
    static objects quests;                  // All the quests
};

#endif // __DATA_H__
