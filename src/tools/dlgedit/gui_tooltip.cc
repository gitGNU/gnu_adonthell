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
 * @file gui_tooltip.cc
 *
 * @author Kai Sterker
 * @brief A custom, tooltip-like widget
 */

#include <gtk/gtk.h>
#include "gui_tooltip.h"
#include "gui_dlgedit.h"

// constructor
GuiTooltip::GuiTooltip (DlgNode *n)
{
    GtkWidget *frame;
    GtkWidget *tip;
    std::string text;

    // the actual tooltip
    tooltip = gtk_window_new (GTK_WINDOW_POPUP);
    gtk_object_set_data (GTK_OBJECT (tooltip), "tip_window", tooltip);
    gtk_window_set_policy (GTK_WINDOW (tooltip), FALSE, FALSE, FALSE);

    // border around the label
    frame = gtk_frame_new (NULL);
    gtk_widget_ref (frame);
    gtk_object_set_data_full (GTK_OBJECT (tooltip), "frame", frame, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame);
    gtk_container_add (GTK_CONTAINER (tooltip), frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);

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
    gtk_widget_ref (tip);
    gtk_object_set_data_full (GTK_OBJECT (tooltip), "tip", tip, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (tip);

    gtk_container_add (GTK_CONTAINER (frame), tip);
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
    
    int x, y;
    gdk_window_get_origin (gtk_widget_get_parent_window (graph), &x, &y);
    gtk_widget_realize (tooltip);
    
    // calculate the position of the tooltip
    if (node->x () < graph->allocation.width / 2) 
        x += node->x () + node->width ();
    else 
        x += node->x () - tooltip->allocation.width; 
    y += node->y () + node->height ();
    
    // position and display the tooltip
    gtk_widget_set_uposition (tooltip, x + offset.x (), y + offset.y ());
    gtk_widget_show (tooltip);
}
