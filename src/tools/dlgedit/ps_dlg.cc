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

ps_dlg::ps_dlg (player *p)
{
    myplayer = p;
}

void ps_dlg::run ()
{
    wnd = create_ps_window (this, myplayer->get_name (), myplayer->get("race"), myplayer->get("gender"));
}

void ps_dlg::on_ok (char* n, int r, int g)
{
    delete[] myplayer->get_name ();  
    myplayer->set_name (n);
    myplayer->set("race", r);
    myplayer->set("gender", g); 
}