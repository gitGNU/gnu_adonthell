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


#include "atk_border_template.h"



atk_border_template::atk_border_template ()
{
    for (int i = 0; i < 8; i++) img_[i] = NULL; 
}


void atk_border_template::load (igzstream is)
{
    // free memory before
    destroy ();
    
    for (int i = 0; i < 8; i++)
    {
        img_[i] = new image;
        img_[i]->get (is); 
    }
}


void atk_border_template::save (ogzstream os)
{
    for (int i = 0; i < 8; i++) 
        img_[i]->put (os); 
}


image * atk_border_template::get (u_int8 i)
{
    return img_[i]; 
}




void atk_border_template::destroy ()
{
    for (int i = 0; i < 8; i++)
    {
        delete img_[i];
        img_[i] = NULL; 
    }
}


atk_border_template::~atk_border_template ()
{
    destroy (); 
}


