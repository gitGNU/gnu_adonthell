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

#include "preset.h"
#include "function.h"
#include "pset_interface.h"

function_dlg::function_dlg (MainFrame *w) : preset_dlg (w)
{
    // Have to change some stuff of the preset dialog here :(
    gtk_window_set_title (GTK_WINDOW (dlg), "Custom Functions");
    gtk_editable_delete_text (GTK_EDITABLE (entry), 0, -1);
    gtk_text_insert (GTK_TEXT (entry), entry->style->font,
        &entry->style->black, &entry->style->white, 
        wnd->cust_func.c_str (), -1);
}

// OK Button hit -> preserve changes
int function_dlg::on_ok (char *v)
{
    wnd->cust_func = v;
    return 1;
}

