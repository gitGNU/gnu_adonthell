/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef __PSDLG_H__
#define __PSDLG_H__

#include <gtk/gtkentry.h>
#include <gtk/gtkstyle.h>
#include <gtk/gtkradiobutton.h>
#include "../../character.h"

class ps_dlg
{
public:
    ps_dlg (character*, character*);

    void run ();
    character *get_npc ();             // Return selected NPC
    void on_ok (char*, int, int, char*);    // OK-Button pressed

    GtkEntry *name;                         // Set player's name
    GtkRadioButton *race;                   // Select the player's race
    GtkRadioButton *gender;                 // Select the player's gender
    GtkWidget *npc_menu;                    // The "chose NPC" option-menu
    GtkWidget *wnd;                         // The dialog window
    
private:
    character *myplayer;               // The player object
    character *mynpc;                  // The NPC object attached to the dialogue
};

#endif // __PSDLG_H__
