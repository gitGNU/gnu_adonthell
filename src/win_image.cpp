/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_theme.h"
#include "win_image.h"

win_image::win_image(s_int16 tx,s_int16 ty,image * tpic,win_theme* them):win_base(tx,ty,tpic->length,tpic->height,them)
{
  picture=new image();
  picture=tpic; //copy image
  // src=tpic;
  // *picture=*src; //copy image into *src
}

win_image::win_image(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme* them):win_base(tx,ty,tl,th,them)
{
  picture=NULL;
  src=NULL;
}

win_image::~win_image()
{
  if(picture) delete picture;
}

void win_image::set_image(image * tpic)
{
  if(picture) delete picture;
  picture=new image();
  *picture=*tpic;
  src=tpic;
}

void win_image::set_stretch(bool b)
{
  stretch_=b;
  update_image();
}

void win_image::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  update_image();
}

void win_image::update_image()
{
  if(picture==NULL || src==NULL) return;
  if(stretch_)
    {
      picture->resize(length_,height_);
      picture->zoom(src);
    }
  else *picture=*src;
}

bool win_image::draw()
{
  if(win_base::draw())
    {
      assign_drawing_area();
      draw_background();
      if(picture)
	if(draw_brightness_)
	  {
	    static image imgbright;
	    imgbright.brightness(picture,level_brightness_);
	    imgbright.draw(realx_,realy_,da_);
	  }
	else picture->draw(realx_,realy_,da_);
      draw_border();
      detach_drawing_area();
      return true;
    }
  return false;
}






