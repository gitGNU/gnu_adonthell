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

#include "error_dlg.h"
#include "err_interface.h"

error_dlg::error_dlg ()
{
    dlg = create_error_window (this);
}

void error_dlg::display (const char *msg)
{
    gtk_editable_delete_text ((GtkEditable *) entry, 0, -1); 
    gtk_text_insert ((GtkText *) entry, entry->style->font,
        &entry->style->black, &entry->style->white, msg, -1);
}

void error_dlg::to_front ()
{
    gdk_window_show (gtk_widget_get_parent_window (entry));
}