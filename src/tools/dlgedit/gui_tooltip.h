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
 * @file gui_tooltip.h
 *
 * @author Kai Sterker
 * @brief A custom, tooltip-like widget
 */

#ifndef GUI_TOOLTIP_H
#define GUI_TOOLTIP_H

#include <gtk/gtkstyle.h>
#include "dlg_node.h"

/**
 * A widget similar to the GtkTooltip that is used to display a
 * DlgCircle's text as long as the mouse hovers over the circle.
 */
class GuiTooltip
{
public:
    GuiTooltip (DlgNode *node);
    ~GuiTooltip ();

    void draw (GtkWidget *parent, DlgPoint &offset);
    
private:
    DlgNode *node;
    GtkWidget *tooltip;
};

#endif // GUI_TOOLTIP_H
