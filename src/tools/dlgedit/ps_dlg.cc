/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "ps_dlg.h"
#include "ps_interface.h"

ps_dlg::ps_dlg (character *p, character *n)
{
    myplayer = p;
    mynpc = n;
}

void ps_dlg::run ()
{
    wnd = create_ps_window (this, myplayer->get_name().c_str (), myplayer->storage::get("race"),
        myplayer->storage::get("gender"), mynpc->get_name().c_str ());
}

void ps_dlg::on_ok (char* n, int r, int g, char *the_npc)
{
    data::characters.erase (myplayer->get_name().c_str ());
    
    myplayer->set_name(n);
    myplayer->set("race", r);
    myplayer->set("gender", g);

    data::characters[myplayer->get_name().c_str ()] = myplayer;
    mynpc = (character *) data::characters[the_npc];
}

character *ps_dlg::get_npc ()
{
    return mynpc;
}
