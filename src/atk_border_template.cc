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


#include <iostream>
#include "atk_border_template.h"



atk_border_template::atk_border_template ()
{

    for (int i = 0; i < _LAST; i++) img_[i] = NULL; 
    for (int j = 0; j < C_TL; j++) imgborder_[j] = NULL; 
}


void atk_border_template::load (igzstream & is)
{
    // free memory before
    destroy ();
    name_ << is ; 
    
    for (int i = 0; i < _LAST; i++)
    {
        img_[i] = new gfx::image;
        img_[i]->get (is); 
    }
    
    for (int i = C_TL; i <_LAST; i++)
      img_[i]->set_mask (true);
    build();
}


void atk_border_template::build()
{
  if (img_[B_TOP] == NULL) return;
  if( imgborder_[B_TOP] == NULL)
    for (int i = 0; i <C_TL; i++)
      {
	imgborder_[i] = new gfx::image;
	imgborder_[i]->set_mask (true);
      }
  
  /* horizontal bar */
  imgborder_[B_TOP]->resize (1280, img_[B_TOP]->height());
  imgborder_[B_TOP]->tile (*(img_[B_TOP]));
  imgborder_[B_BOTTOM]->resize (1280, img_[B_BOTTOM]->height());
  imgborder_[B_BOTTOM]->tile (*(img_[B_BOTTOM]));
  
  /* vertical bar */
  imgborder_[B_LEFT]->resize (img_[B_LEFT]->length(), 1280);
  imgborder_[B_LEFT]->tile (*(img_[B_LEFT]));
  imgborder_[B_RIGHT]->resize (img_[B_RIGHT]->length(), 1280);
  imgborder_[B_RIGHT]->tile (*(img_[B_RIGHT]));
}




void atk_border_template::save (ogzstream & os)
{
    name_ >> os; 
    for (int i = 0; i < _LAST; i++) 
        img_[i]->put (os); 
}


gfx::image * atk_border_template::get (u_int8 i)
{
  if ( i>= _LAST) return NULL;
  return img_[i]; 
}


gfx::image * atk_border_template::get_border (u_int8 i)
{
  /* WARNING this test can be removed to optimize more .... */
  if ( i>= C_TL) return NULL;
  
  return imgborder_[i];
}

void atk_border_template::set (u_int8 type, gfx::image * img)
{
    if (img_[type]  != NULL) delete img_[type];
    img_[type] =  img; 
}

void atk_border_template::set (u_int8 type, std::string filename)
{
  gfx::image * img = new gfx::image;
  if ( img->load_pnm (filename) != 0)
    std::cout << "Error on load image " << filename <<  " in atk_border_template::set \n"; 
  set (type, img); 
}


void atk_border_template::destroy ()
{
    for (int i = 0; i < _LAST; i++)
    {
        if (img_[i]) delete img_[i];
        img_[i] = NULL; 
    }

    for (int j = 0; j < C_TL; j++)
      {
	if (imgborder_[j]) delete imgborder_[j];
	imgborder_[j] = NULL;
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






