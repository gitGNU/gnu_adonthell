/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _win_query_h
#define _win_query_h

#include <list>
#include <string>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_scrollbar.h"
#include "win_theme.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"

class win_query : public win_container
{
  win_label * label;
  win_write * entry;
 public:
  win_query(u_int16 x, u_int16 y, win_theme * th, win_font * font,
	    const char * mytext);

  char * wait_for_text(const Functor0 & updatefunc,
		       const Functor0 & drawfunc);
};

#endif
