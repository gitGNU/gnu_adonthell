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

#ifndef WIN_IMAGE_H_
#define WIN_IMAGE_H_


class win_base;
class image;
class win_container;
class drawing_area;

class win_image : public win_base
{
  
  image * picture;
 public:
  win_image(u_int16,u_int16,image * tpic,win_container * wc);
  void set_image(image *);
  void draw();
  void update();
  ~win_image();
};
#endif
