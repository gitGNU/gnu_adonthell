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

#ifndef __RUN_H__
#define __RUN_H__

#include <gtk/gtkstyle.h>

// Wrapper class for the Run Dialogue dialog
class run_dlg
{
public:
    run_dlg ();

    GtkWidget *dlg;             // The actual Dialogue Window
    GtkWidget *list;            // The List widget that holds the dialogue text
private:
};

#endif // __RUN_H__
