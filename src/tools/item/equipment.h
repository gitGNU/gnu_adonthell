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
 * @file   equipment.h
 * @author Thorsten Riess <thorsten.riess@tu-ilmenau.de>
 * 
 * @brief  Class that manages the equipped items of a character
 */

#ifndef _EQUIPMENT_H
#define _EQUIPMENT_H

#include <vector>
class character_base;
class item_base;

enum equip_pos { LEFT_HAND, RIGHT_HAND, BOTH_HANDS, HEAD, 
    LEGS, BODY, RING };

class equipment {
public:
    equipment ();
#ifndef SWIG
    equipment ( character_base * c );
#endif
    void set_character ( character_base * c ) { character = c; };
    character_base * get_character () { return character; };
    bool equip ( item_base * item, equip_pos position );
    bool unequip ( item_base * item );

private:
    character_base * character;
    item_base * left_hand;
    item_base * right_hand;
    item_base * head;
    item_base * legs;
    item_base * body;
    vector<item_base *> rings;
};

#endif
