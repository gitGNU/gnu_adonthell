/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>

class main_wnd
{
public:
    main_wnd ();

    void load_default_attributes (void);

    GtkWidget *name_entry;
    GtkWidget *race_choice;
    GtkWidget *gender_choice;
    GtkWidget *attrib_entry;
    GtkWidget *val_entry;
    GtkWidget *attribute_list;
    int attribute_list_sel;

    GtkWidget *event_choice;
    GtkWidget *event_list;

    GtkWidget *dlg_list;
};

#endif // __MAIN_H__