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

#include "win_query.h"
#include "input.h"

win_query::win_query (u_int16 x, u_int16 y, win_theme * th, win_font * font,
                      const char *mytext):win_container (x, y, 200, 120, th)
{
    label = new win_label (5, 5, 190, 20, th, font);
    entry = new win_write (5, 30, 190, 75, th, font);
    label->set_text (mytext);
    add (label);
    add (entry);
    set_border_visible (true);
    set_background_visible (true);
    set_visible_all (true);
    entry->set_activated (true);
    entry->set_activate_keyboard (true);
    entry->set_focus (true);
}

string win_query::wait_for_text (const Functor0 & updatefunc,
                                 const Functor0 & drawfunc)
{
    u_int16 i;
    string s;

    set_signal_connect (updatefunc, WIN_SIG_UPDATE);
    set_signal_connect (drawfunc, WIN_SIG_DRAW);

    input::clear_keys_queue ();
    input::set_key_repeat ();
    while (!entry->is_text ())
    {
        input::update ();
        if (input::has_been_pushed (SDLK_ESCAPE))
        {
            s = "Aborded";
            return s;
        }
        for (i = 0; i < screen::frames_to_do (); i++)
            update ();
        entry->update ();
        draw ();
        screen::show ();
    }
    input::set_key_repeat (0);
    input::clear_keys_queue ();
    s = entry->get_text ();
    return s;
}
