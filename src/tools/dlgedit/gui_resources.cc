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

    GdkWindow     *window = gtk_widget_get_parent_window (widget);
    GdkColormap *colormap = gtk_widget_get_colormap (widget);
  
    // Black
    Color[GC_BLACK] = gdk_gc_new (window);
    c.red   = 0;
    c.green = 0;
    c.blue  = 0;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_BLACK], &c);
    
    // White
    Color[GC_WHITE] = gdk_gc_new (window);
    c.red   = 65535;
    c.green = 65535;
    c.blue  = 65535;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_WHITE], &c);

    // Grey
    Color[GC_GREY] = gdk_gc_new (window);
    c.red   = 50000;
    c.green = 50000;
    c.blue  = 50000;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_GREY], &c);

    // Dark Red
    Color[GC_DARK_RED] = gdk_gc_new (window);
    c.red   = 65535;
    c.green = 0;
    c.blue  = 0;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_RED], &c);

    // Red
    Color[GC_RED] = gdk_gc_new (window);
    c.red   = 65535;
    c.green = 16350;
    c.blue  = 0;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_RED], &c);

    // Green
    Color[GC_GREEN] = gdk_gc_new (window);
    c.red   = 39680;
    c.green = 65355;
    c.blue  = 45900;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_GREEN], &c);

    // Dark Green
    Color[GC_DARK_GREEN] = gdk_gc_new (window);
    c.red   = 0;
    c.green = 27300;
    c.blue  = 15600;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_GREEN], &c);

    // Bright Green
    Color[GC_BRIGHT_GREEN] = gdk_gc_new (window);
    c.red   =     0;
    c.green = 47616;
    c.blue  =  1536;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_BRIGHT_GREEN], &c);

    // Dark Blue
    Color[GC_DARK_BLUE] = gdk_gc_new (window);
    c.red   = 0;
    c.green = 0;
    c.blue  = 60000;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_DARK_BLUE], &c);

    // Blue - like the sky
    Color[GC_BLUE] = gdk_gc_new (window);
    c.red   = 32768;
    c.green = 58112;
    c.blue  = 65535;
    gdk_colormap_alloc_color (colormap, &c, TRUE, TRUE);
    gdk_gc_set_foreground (Color[GC_BLUE], &c);

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
            if (type == NPC || type == LINK || type == MODULE) 
                                       return Color[GC_BLACK];
            else if (type == NARRATOR) return Color[GC_DARK_GREEN];
            else if (type == PLAYER)   return Color[GC_DARK_BLUE];
            else                       return Color[GC_BLACK];
        }

        // selected
        case NODE_SELECTED:
        {
	    return Color[GC_DARK_RED];
        }

        // highlighted
        case NODE_HILIGHTED:
        {
            return Color[GC_BRIGHT_GREEN];
        }

        // unknown
        default:
            return Color[GC_BLACK];
    }

    return (GdkGC *) NULL;
}

// Get the right Fill Color for a (mode, type) combination
// This color is going to be filled inside the node cirle
GdkGC *GuiResources::getFill (mode_type mode, node_type type)
{    
    switch (type)
    {
        case PLAYER:
	    return Color[GC_BLUE];
        case NPC:
	    return Color[GC_GREY];
        case NARRATOR:
	    return Color[GC_GREEN];
        // unknown
        default:
            return Color[GC_BLACK];
    }

    return (GdkGC *) NULL;
}
