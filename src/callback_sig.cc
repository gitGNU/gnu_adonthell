/*
   $Id$

   (C) Copyright 2002 Joel Vennin

   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#include "callback_sig.h"
 
void callback_sig::operator () ()
{
    for (std::vector <callback_slot *>::iterator it = lslot.begin (); it != lslot.end (); it++)
        (**it) (); 
}


void callback_sig::connect (callback_slot * s) 
{
    lslot.push_back (s);
}


void callback_sig::clear ()
{
    for (std::vector <callback_slot *>::iterator it = lslot.begin (); it != lslot.end (); it++)
        delete *it; 
    lslot.clear (); 
}


callback_sig::~callback_sig ()
{
    clear (); 
}


