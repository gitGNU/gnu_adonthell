/*
   Copyright (C) 2000/2001 Kai Sterker <kai.sterker@gmail.com>
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
    extern dictionary <quest *> quests;
} 
#endif

#endif // __QUEST_H__
