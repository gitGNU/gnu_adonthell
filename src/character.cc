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
 * @file   character.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the character class.
 * 
 * 
 */

#include "character.h"
 
dictionary <character *> data::characters;
character * data::the_player = NULL;

character::character()
{
}

character::~character()
{
}
