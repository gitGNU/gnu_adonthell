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


/**
 * @file   character.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the character class.
 * 
 * 
 */

 
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "mapcharacter.h"

/**
 * Class holding %game characters.
 * 
 */ 
class character : public mapcharacter
{
public:
    /** 
     * Default constructor.
     * 
     */
    character();

    /**
     * Destructor.
     * 
     */ 
    ~character();
};

#ifndef SWIG

namespace data
{
    /**
     * All the game's characters, different from the player.
     *
     */
    extern dictionary <character *>  characters;

    /**
     * The player character.
     *
     */
    extern character *the_player;
}
#endif

#endif // CHARACTER_H_
