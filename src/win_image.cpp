/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <list>
#include "types.h"
#include "image.h"
#include "win_base.h"

#include "win_container.h"
#include "win_select.h"
#include "win_image.h"


win_image::win_image(u_int16 tx,u_int16 ty,image * tpic,win_container * twc):win_base(tx,ty,0,0,twc,twc->get_drawing_area())
{
  picture=tpic;
  if(picture)
    {
      height=picture->height;
      length=picture->length;
    }
}

win_image::~win_image()
{
  if(wselect)
    {
       wselect->remove(this);
       wselect=NULL;
     }
}

void win_image::set_image(image * tpic)
{
  picture=tpic;
  if(picture)
    {
      height=picture->height;
      length=picture->length;
    }
}

void win_image::draw()
{
  if(visible && wc && picture)
    {
       picture->draw(real_x,real_y,wc->get_drawing_area());
       draw_border();
    }
}

void win_image::update()
{

}
