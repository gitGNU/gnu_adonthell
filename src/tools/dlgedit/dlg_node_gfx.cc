/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_node_gfx.cc
 *
 * @author Kai Sterker
 * @brief Base class for a DlgNode's graphical representation.
 */

#include <gtk/gtk.h>
#include "dlg_node_gfx.h"

// blit part of widget to the screen
void DlgNodeGfx::update (GtkWidget *widget, DlgRect &area)
{
    if (widget)
    {
        GdkRectangle rect = (GdkRectangle) area;
        gtk_widget_draw (widget, &rect);
    }
}
