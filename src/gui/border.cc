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


#include "border.h"

using namespace gui;

border::border (container * ac) : container_ (ac), btempl_ (NULL) 
{    
    /*init tab */
    for (int i = 0; i < border_template::_LAST; i++)
    {
        img_[i] = NULL; 
        imgbright_[i] = NULL; 
    }
    
    /* by default use image without brightness */
    imguse_ = img_; 
}


void border::draw (drawing_area* da = NULL, surface * sf = NULL)
{
    if (!btempl_) return;  
}


void border::resize ()
{
    if (!btempl_) return; 
}


void border::set_border (border_template * bt)
{
    btempl_ = bt;

    /* free memory */
    clear (); 
    
    /* resize*/
    resize (); 
}



void border::clear ()
{
    for (int i = 0; i < border_template::_LAST; i++)
    {
        delete img_[i];
        img_[i] = NULL;
        
        delete imgbright_[i];
        imgbright_[i] = NULL;
    }
}


border_template * border::get_template () const
{
    return btempl_; 
}











