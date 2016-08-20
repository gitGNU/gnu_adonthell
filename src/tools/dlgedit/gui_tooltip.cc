/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
*/

/** 
 * @file gui_tooltip.cc
 *
 * @author Kai Sterker
 * @brief A custom, tooltip-like widget
 */

#include "gui_dlgedit.h"
#include "gui_tooltip.h"

// constructor
GuiTooltip::GuiTooltip (DlgNode *n)
{
    GtkWidget *tip;
    std::string text;

    // the actual tooltip
    tooltip = gtk_window_new (GTK_WINDOW_POPUP);
    gtk_window_set_keep_above (GTK_WINDOW (tooltip), FALSE);
    gtk_window_set_transient_for (GTK_WINDOW (tooltip), GTK_WINDOW (GuiDlgedit::window->getWindow ()));
    gtk_window_set_type_hint (GTK_WINDOW (tooltip), GDK_WINDOW_TYPE_HINT_NOTIFICATION);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW (tooltip), TRUE);
    gtk_window_set_opacity (GTK_WINDOW (tooltip), 0.9f);
    
    g_object_set_data (G_OBJECT (tooltip), "tip_window", tooltip);
    gtk_window_set_resizable (GTK_WINDOW (tooltip), FALSE);

    // get the text
    switch (n->type ())
    {
        case NPC:
        case PLAYER:
        case NARRATOR:
        {
            text = ((DlgCircle *) n)->tooltip ();
            break;            
        }
        case MODULE:
        {
            text = ((DlgModule *) n)->entry ()->description ();
            break;            
        }
        default: break;
    }
    
    // label with the text
    tip = gtk_label_new (text.c_str ());
    g_object_ref (tip);
    g_object_set_data_full (G_OBJECT (tooltip), "tip", tip, (GDestroyNotify)  g_object_unref);
    gtk_widget_show (tip);

    gtk_container_add (GTK_CONTAINER (tooltip), tip);
    gtk_label_set_justify (GTK_LABEL (tip), GTK_JUSTIFY_LEFT);
    gtk_label_set_line_wrap (GTK_LABEL (tip), TRUE);
    gtk_misc_set_padding (GTK_MISC (tip), 4, 1);
    
    // keep a pointer to the node
    node = n;    
}

// destructor
GuiTooltip::~GuiTooltip ()
{
    gtk_widget_destroy (tooltip);
}

// draw the tooltip
void GuiTooltip::draw (GtkWidget *graph, DlgPoint &offset)
{
    if (node->type () == LINK) return;

    // get position and extension of dlgedit window    
    int x, y;

    GdkWindow *window = gtk_widget_get_parent_window (graph);
    gdk_window_get_origin (window, &x, &y);
    int width = gdk_window_get_width(window);
    
    gtk_widget_realize (tooltip);

    GtkAllocation allocation;
    gtk_widget_get_allocation (graph, &allocation);
    
    // calculate the position of the tooltip
    x += width - allocation.width;
    if (node->x () < allocation.width / 2)
    {
        x += node->x () + node->width ();
    }
    else
    {
        gtk_widget_get_allocation (tooltip, &allocation);
        x += node->x () - allocation.width;
    }
    y += node->y () + node->height ();
    
    // position and display the tooltip
    gtk_window_move (GTK_WINDOW(tooltip), x + offset.x (), y + offset.y ());
    gtk_widget_show (tooltip);
}
