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

#ifndef GUI_THEME_H_
#define GUI_THEME_H_

#include "border.h"
#include "background.h"
#include "scroll.h"
#include "scrollbar.h"

namespace gui {

  class theme
    {
    public:
      theme();
      
      theme(char *);
  
      theme(theme &);
      
      theme & operator=(theme &);
      
      ~theme();
      
      void destroy();
      
      friend class border;
      friend class background;
      friend class scrollbar;
      
    private:
      
      border * my_border_normal;
      
      border * my_border_mini;
      
      background * my_background;
      
      scrollbar * my_scrollbar;
    };
}
#endif
