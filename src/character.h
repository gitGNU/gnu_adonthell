/*
   $Id$
   
   Copyright (C) 2000/2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   character.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the character class.
 * 
 * 
 */

 
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "storage.h"

#ifdef USE_MAP
#include "mapcharacter.h"
#else
#include "character_base.h"
#endif

/**
 * Class holding %game characters.
 * 
 */ 
class character
#ifdef USE_MAP
: public mapcharacter
#else
: public character_base
#endif
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
