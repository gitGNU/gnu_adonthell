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

#ifndef _INVENTORY_CHAR_H
#define _INVENTORY_CHAR_H

#include "inventory_base.h"

class inventory_char : public inventory_base
{
public:
    inventory_char ( character_base * c ) : inventory_base ();
    ~inventory_char ();

    bool add_money ( u_int16 v ) { money_+=v; return true; };
    bool sub_money ( u_int16 v ) 
        { if (money_<v) return false; 
            money_-=v; 
            return true; 
        };
    
    u_int16 money ();
    
protected:
    u_int16 money_;
};

#endif
