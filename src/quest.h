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

#ifndef __QUEST_H__
#define __QUEST_H__

#include "fileops.h"
#include "storage.h"

// This class stores flags and variables defining the player's
// progress in a specific quest
class quest : public storage
{
public:
    quest ();
    ~quest ();
    
#ifndef SWIG
    void load (igzstream&);
    void save (ogzstream&);

    string name;
#endif // SWIG
};


#ifndef SWIG 
namespace data
{
    /**
     * All the quests.
     * 
     */ 
    extern dictionnary <quest *> quests;
} 
#endif

#endif // __QUEST_H__
