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

#include "atk_bin.h"

atk_bin::atk_bin () : child (NULL)
{ 
}



void atk_bin::add (atk_widget * w)
{
    if (child) remove (child); 

    child = w;
    w->set_parent (this); 

    on_add (); 
}



void atk_bin::remove (atk_widget *w)
{
    w->set_parent (NULL);

    child = NULL;

    on_remove (); 
}


void atk_bin::clear ()
{
    if (child) 
    {
        delete child; 
        child = NULL;
    }
}


atk_bin::~atk_bin ()
{
    clear (); 
}
