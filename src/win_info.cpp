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

#include "win_info.h"
#include "input.h"

win_info::win_info(u_int16 x, u_int16 y, win_theme * th, win_font * font,
		   const char * mytext)
  : win_container(x,y,200,120,th)
{
  label=new win_label(5,5,190,130,th,font);
  label->set_text(mytext);
  label2=new win_label(60,90,80,15,th,font);
  label2->set_text("Press any key...");
  add(label);
  add(label2);
  set_visible_all(true);
  set_border_visible(true);
  set_background_visible(true);
}

void win_info::wait_for_keypress(const Functor0 & updatefunc,
				 const Functor0 & drawfunc)
{
  u_int16 i;
  set_signal_connect(updatefunc,WIN_SIG_UPDATE);
  set_signal_connect(drawfunc,WIN_SIG_DRAW);
  input::clear_keys_queue();
  do
    {
      input::update();
      for(i=0;i<screen::get_frames_to_do();i++) update();
      draw();
      screen::show();
    }
  while(input::get_next_key()<0);
  input::clear_keys_queue();
}
