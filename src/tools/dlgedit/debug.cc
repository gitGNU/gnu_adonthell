/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#include <gtk/gtk.h>
#include "debug.h"
#include "dbg_interface.h"

debug_dlg::debug_dlg ()
{
    dlg = create_debug_wnd (this);
}

void debug_dlg::update ()
{
}