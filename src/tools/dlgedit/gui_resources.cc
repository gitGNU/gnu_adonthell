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
 * @file gui_resources.cc
 *
 * @author Kai Sterker
 * @brief Provides fonts and Colors used throughout dlgedit.
 */

#include <gtk/gtk.h>
#include "gui_resources.h"

/**
 * The font for text output.
 */
GdkFont *GuiResources::Font;

/**
 * Some pens for line drawing.
 */
GdkGC *GuiResources::Color[MAX_GC];

// create font and pens
void GuiResources::init (GtkWidget *widget)
{
    // font to use on the drawing area
    Font = gdk_font_load ("-*-*-medium-r-normal-sans-12-*-*-*-*-*-iso8859-1");

    GdkColor c;

    // Yellow
    Color[GC_YELLOW] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 65535;
    c.green = 32700;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_YELLOW], &c);

    // Orange
    Color[GC_RED] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 65535;
    c.green = 16350;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_RED], &c);

    // Dark Red
    Color[GC_DARK_RED] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 65535;
    c.green = 0;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_RED], &c);

    // Dark Blue
    Color[GC_DARK_BLUE] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 0;
    c.green = 0;
    c.blue = 35000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_BLUE], &c);

    // Green
    Color[GC_GREEN] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 16000;
    c.green = 50000;
    c.blue = 5000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_GREEN], &c);

    // Dark Green
    Color[GC_DARK_GREEN] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 0;
    c.green = 27300;
    c.blue = 15600;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_GREEN], &c);

    // Grey
    Color[GC_GREY] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 40000;
    c.green = 40000;
    c.blue = 40000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_GREY], &c);

    // Black
    Color[GC_BLACK] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 0;
    c.green = 0;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_BLACK], &c);
    
    // White
    Color[GC_WHITE] = gdk_gc_new (gtk_widget_get_parent_window (widget));
    c.red = 65535;
    c.green = 65535;
    c.blue = 65535;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (widget), &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_WHITE], &c);
}

// Get a certain predefined Color
GdkGC *GuiResources::getColor (int color)
{
    if (color < 0 || color > MAX_GC) return (GdkGC *) NULL;
    else return Color[color];
}

// Get the right Color for a (mode, type) combination
GdkGC *GuiResources::getColor (mode_type mode, node_type type)
{    
    switch (mode)
    {
        // not selected    
        case IDLE:
        {
            if (type == NPC || type == LINK || type == MODULE) return Color[GC_BLACK];
            else if (type == NARRATOR) return Color[GC_DARK_GREEN];
            else return Color[GC_DARK_BLUE];
        }

        // selected
        case NODE_SELECTED:
        {
            if (type == NPC || type == LINK || type == MODULE) return Color[GC_DARK_RED];
            else if (type == NARRATOR) return Color[GC_YELLOW];
            else return Color[GC_RED];
        }

        // highlighted
        case NODE_HILIGHTED:
        {
            return Color[GC_GREEN];
        }

        // unknown
        default:
            return Color[GC_BLACK];
    }

    return (GdkGC *) NULL;
}
