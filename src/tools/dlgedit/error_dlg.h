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

#ifndef __ERROR_DLG_H__
#define __ERROR_DLG_H__

#include <gtk/gtkstyle.h>

// Set variables to be used for testing the Dialogue
class error_dlg
{
public:
    error_dlg ();

    GtkWidget *dlg;             // The actual Dialogue Window
    GtkWidget *entry;           // The Text entry

    void to_front ();           // Bring dialog to front
    void display (const char*); // Display some text
};

#endif // __ERROR_DLG_H__
