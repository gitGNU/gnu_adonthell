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
#include "../../data.h"

ps_dlg::ps_dlg (player *p, npc *n)
{
    myplayer = p;
    mynpc = n;
}

void ps_dlg::run ()
{
    wnd = create_ps_window (this, myplayer->name, myplayer->get("race"),
        myplayer->get("gender"), mynpc->name);
}

void ps_dlg::on_ok (char* n, int r, int g, char *the_npc)
{
    delete myplayer->name;  
    myplayer->name = strdup (n);
    myplayer->set("race", r);
    myplayer->set("gender", g);

    mynpc = (npc *) data::characters.get (the_npc);
}

npc *ps_dlg::get_npc ()
{
    return mynpc;
}