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
 * @file gui_resources.h
 *
 * @author Kai Sterker
 * @brief Provides fonts and colors used throughout dlgedit.
 */

#ifndef GUI_RESOURCES_H
#define GUI_RESOURCES_H

#include <gtk/gtkstyle.h>
#include "dlg_types.h"

/**
 * This class provides some fonts and pens that are used throughout
 * dlgedit. Instead of creating the newly each time something needs
 * to be drawn, they are created once when the program starts.
 */
class GuiResources
{
public:
    /**
     * Create all the fonts and colors needed.
     * @param widget the widget we later draw to 
     */
    static void init (GtkWidget *widget);

    /**
     * Retrieve a colored 'pen' to draw onto a drawing area.
     * @param color the color to retrieve
     * @return a GdkGC.
     */
    static GdkGC *getColor (int color);
    /**
     * Retrieve a colored 'pen' to draw onto a drawing area, depending
     * on the given mode and type.
     * @param mode the mode of a DlgNode
     * @param type the type of a DlgNode
     * @return a GdkGC.
     */
    static GdkGC *getColor (mode_type mode, node_type type);
    /**
     * Retrieve a color to fill drawn objects on a drawing area, 
     * depending on the given mode and type.
     * @param mode the mode of a DlgNode
     * @param type the type of a DlgNode
     * @return a GdkGC.
     */
    static GdkGC *getFill (mode_type mode, node_type type);
    /**
     * Retrieve the font used to draw text onto a drawing area.
     * @return a GdkFont.
     */
    static GdkFont *font ()         { return Font; }

private:
    static GdkFont *Font;           // font for text-output
    static GdkGC *Color[MAX_GC];    // custom Pens    
};

#endif // GUI_RESOURCES_H
