/*
   $Id$
       
   Copyright (C) 2002 Thorsten Riess <thorsten.riess@tu-ilmenau.de>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   equipment.cc
 * @author Thorsten Riess <thorsten.riess@tu-ilmenau.de>
 * 
 * @brief  Class that manages the equipped items of a character
 */

#include "equipment.h"
#include "character_base.h"
#include "item_base.h"

equipment::equipment ()
{
    character = NULL;
}

equipment::equipment ( character_base * c )
{
    character = c;
}

bool equipment::equip ( item_base * item, equip_pos position )
{
    switch ( position )
    {
    case LEFT_HAND:
        left_hand = item;
        break;
    case RIGHT_HAND:
        right_hand = item;
        break;
    case BOTH_HANDS:
        left_hand = right_hand = item;
        break;
    case HEAD:
        head = item;
        break;
    case LEGS:
        legs = item;
        break;
    case BODY:
        body = item;
        break;
    case RING:
        rings.push_back ( item );
        break;
    }
    return false;
}

bool equipment::unequip ( item_base * item )
{
    if ( left_hand == item )
        left_hand = NULL;
    if ( right_hand == item )
        right_hand = NULL;
    if ( head == item )
        head = NULL;
    if ( legs == item )
        legs = NULL;
    if ( body == item )
        body = NULL;
    vector<item_base *>::iterator iter=find( rings.begin(), rings.end(), item);
    if ( iter != rings.end () )
        rings.erase ( iter );
    return false;
}
