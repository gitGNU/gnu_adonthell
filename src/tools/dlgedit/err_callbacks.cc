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

#include <gtk/gtk.h>

#include "error_dlg.h"
#include "err_callbacks.h"

void
on_error_close_clicked (GtkButton *button, gpointer user_data)
{
    error_dlg *dlg = (error_dlg *) user_data;
    
    gtk_widget_destroy (dlg->dlg);

    delete dlg;
}

void
on_error_delete (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    error_dlg *dlg = (error_dlg *) user_data;
    
    gtk_widget_destroy (widget);

    delete dlg;
}
