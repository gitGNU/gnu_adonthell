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

#ifndef WIN_BORDER_H_
#define WIN_BORDER_H_

#include "win_types.h"
class image;
class win_container;

class win_border
{
  //image * h_border;
  //image * v_border;

 public:
  image * h_border_template;
  image * v_border_template;
  image * corner;

  win_border(char *rep ,char *size=WIN_BORDER_NORMAL_SIZE);
  ~win_border();
  void load(char *,char *);
};


class win_background
{
 public:
  image * background_template;
  win_background(char *);
  ~win_background();
  void load(char *);
};

class win_cursor
{
 public:
  image * cursor;
  win_cursor(char *);
  ~win_cursor();
  void load(char *);
};


class win_scrollbar
{
 public:
  image * bar;
  image * bottom;
  image * middle;
  image * top;
  win_scrollbar(char *);
  void load(char *);
  ~win_scrollbar();
};
#endif




