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
 * @file gui_circle.h
 *
 * @author Kai Sterker
 * @brief The Edit Circle dialog
 */

#ifndef GUI_CIRCLE_H
#define GUI_CIRCLE_H

#include <string>
#include <gtk/gtkstyle.h>
#include "dlg_circle_entry.h"
#include "dlg_types.h"
#include "gui_edit.h"

/**
 * This is the dialog used to input text, conditions and arbitrary code of
 * a DlgCircle. It further is used to specify whether the node contains speech
 * of the Player, the Narrator or of an NPC. In latter case, the text may
 * be attached to a special NPC, or to the default one. This default one is
 * always the NPC to whom the dialogue belongs in the game.
 */
class GuiCircle
{
public:
    /**
     * Open the "Edit Dialogue Node" dialog. 
     * @param type The type of the circle to display (NPC, PLAYER or NARRATOR)
     * @param entry The contents of the circle
     */
    GuiCircle (node_type type, DlgCircleEntry *entry);
    ~GuiCircle ();

private:
    GuiEdit *cond_edit;
    GuiEdit *code_edit;
    GtkWidget *dialog;              // The dialog window
    GtkWidget *npc_selection;       // The dropdown list with the various NPC's
    DlgCircleEntry *entry;          // The data to display
};

#endif // GUI_CIRCLE_H
