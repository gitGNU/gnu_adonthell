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

#include "atk_fixed.h"


atk_fixed::atk_fixed ()
{ 
}


void atk_fixed::add (atk_widget * w)
{
    w->set_parent (this);

    v_widget_.push_back (w);

    on_add ();
}


void atk_fixed::remove (atk_widget * w)
{ 
    std::vector <atk_widget * >::iterator it = v_widget_.begin ();
    
    while (it != v_widget_.end () && * it != w)it++;

    if (it != v_widget_.end ())
    {
        v_widget_.erase (it);
        w->set_parent (NULL); 
    }

    on_remove (); 
}


void atk_fixed::clear ()
{
    for ( std::vector <atk_widget * >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++) 
        delete *it;
    clear ();  
}



void atk_fixed::realize ()
{ 
}


atk_fixed::~atk_fixed ()
{
    // free memory
    clear (); 
}


void atk_fixed::draw (drawing_area * da = NULL, surface * sf = NULL)
{
    if (!is_visible ()) return; 
    
    atk_widget::draw (da, sf);

    for (std::vector <atk_widget* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->draw (da, sf);  
}




