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
    for (int i = 0; i < _LAST; i++) img_[i] = NULL; 
}


void atk_border_template::load (igzstream & is)
{
    // free memory before
    destroy ();
    name_ << is ; 
    
    for (int i = 0; i < _LAST; i++)
    {
        img_[i] = new image;
        img_[i]->get (is); 
    }
}


void atk_border_template::save (ogzstream & os)
{
    name_ >> os; 
    for (int i = 0; i < _LAST; i++) 
        img_[i]->put (os); 
}


image * atk_border_template::get (u_int8 i)
{
    return img_[i]; 
}


void atk_border_template::set (u_int8 type, image * img)
{
    if (img_[type]  != NULL) delete img_[type];
    img_[type] =  img; 
}


void atk_border_template::set (u_int8 type, std::string filename)
{
    image * img = new image;
    if ( img->load_pnm (filename) < 0)
        std::cout << "Error on load image " << filename <<  " in atk_border_template::set \n"; 
    set (type, img); 
}


void atk_border_template::destroy ()
{
    for (int i = 0; i < _LAST; i++)
    {
        delete img_[i];
        img_[i] = NULL; 
    }
}


atk_border_template::~atk_border_template ()
{
    destroy (); 
}


std::string atk_border_template::get_name () const
{
    return name_; 
}


void atk_border_template::set_name (const std::string & name)
{
    name_ =  name; 
}


void atk_border_template::display_info () const
{
    std::cout << "border name : " << name_ << std::endl;
    std::cout << "List of image : \n";

    for (int i =  0; i < _LAST;  i++)
        if ( img_[i]  !=  NULL) std::cout << "image " << i << " : " <<  img_[i]->length ()
                                          << "x" << img_[i]->height () << std::endl; 
    
}






