/*
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
class win_theme;
class image;

class win_image : public win_base
{
 private:
  image * src;
  image * picture;
  bool stretch_;
  void update_image();
 public:
  win_image(s_int16 tx,s_int16 ty,image * tpic,win_theme*);
  win_image(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme*);
  ~win_image();
  void resize(u_int16,u_int16);
  void set_image(image *);
  void draw();
  void set_stretch(bool b);
  bool is_stretch(){return stretch_;}
};

#endif
