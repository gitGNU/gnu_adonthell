/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __PRESET_H__
#define __PRESET_H__

#include <gtk/gtkstyle.h>
#include <string>
#include "main.h"

// Set variables to be used for testing the Dialogue
class preset_dlg
{
public:
    preset_dlg (MainFrame*);

    GtkWidget *dlg;             // The actual Dialogue Window
    GtkWidget *entry;           // The Text entry

    MainFrame *wnd;             // The Application

    void run ();
    virtual int on_ok (char*);  // Accepted changes    
};

#endif // __PRESET_H__