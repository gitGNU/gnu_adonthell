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
#include "border_template.h"

using namespace gui;


border_template::border_template ()
{

    for (int i = 0; i < _LAST; i++) img_[i] = NULL; 
    for (int j = 0; j < C_TL; j++) imgborder_[j] = NULL; 
}


void border_template::load (igzstream & is)
{
    // free memory before
    destroy ();
    my_name << is ; 
    
    for (int i = 0; i < _LAST; i++)
    {
        img_[i] = new gfx::image;
        img_[i]->get (is); 
    }
    
    for (int i = C_TL; i <_LAST; i++)
      img_[i]->set_mask (true);
    build();
}


void border_template::build()
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




void border_template::save (ogzstream & os)
{
    my_name >> os; 
    for (int i = 0; i < _LAST; i++) 
        img_[i]->put (os); 
}


gfx::image * border_template::get (u_int8 i)
{
  if ( i>= _LAST) return NULL;
  return img_[i]; 
}


gfx::image * border_template::get_border (u_int8 i)
{
  /* WARNING this test can be removed to optimize more .... */
  if ( i>= C_TL) return NULL;
  
  return imgborder_[i];
}

void border_template::set (u_int8 type, gfx::image * img)
{
    if (img_[type]  != NULL) delete img_[type];
    img_[type] =  img; 
}

void border_template::set (u_int8 type, std::string filename)
{
  gfx::image * img = new gfx::image;
  if ( img->load_pnm (filename) != 0)
    std::cout << "Error on load image " << filename <<  " in border_template::set \n"; 
  set (type, img); 
}


void border_template::destroy ()
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


border_template::~border_template ()
{
    destroy (); 
}





void border_template::display_info ()
{
    std::cout << "border name : " << my_name << std::endl;
    std::cout << "List of image : \n";

    for (int i =  0; i < _LAST;  i++)
        if ( img_[i]  !=  NULL) std::cout << "image " << i << " : " <<  img_[i]->length ()
                                          << "x" << img_[i]->height () << std::endl; 
    
}






