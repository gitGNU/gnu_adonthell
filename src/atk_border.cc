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


#include "atk_border.h"


atk_border::atk_border (atk_container * ac) : container_ (ac), btempl_ (NULL) 
{    
    /*init tab */
    for (int i = 0; i < atk_border_template::_LAST; i++)
    {
        img_[i] = NULL; 
        imgbright_[i] = NULL; 
    }
    
    /* by default use image without brightness */
    imguse_ = img_; 
}


void atk_border::draw (drawing_area* da = NULL, surface * sf = NULL)
{
    if (!btempl_) return;  
}


void atk_border::resize ()
{
    if (!btempl_) return; 
}


void atk_border::set_border (atk_border_template * bt)
{
    btempl_ = bt;

    /* free memory */
    clear (); 
    
    /* resize*/
    resize (); 
}



void atk_border::clear ()
{
    for (int i = 0; i < atk_border_template::_LAST; i++)
    {
        delete img_[i];
        img_[i] = NULL;
        
        delete imgbright_[i];
        imgbright_[i] = NULL;
    }
}


atk_border_template * atk_border::get_template () const
{
    return btempl_; 
}











