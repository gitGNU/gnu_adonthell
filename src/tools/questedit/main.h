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

#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>

class main_wnd
{
public:
    main_wnd ();

    void write_character_source ();
    void read_character_source (gchar*);
    void load_list_defaults (GtkCList *, gchar *, gchar *);
    void colorify_list (GtkCList *);

    GtkWidget *name_entry;
    GtkWidget *attrib_entry;
    GtkWidget *val_entry;
    GtkWidget *attribute_list;
    int attribute_list_sel;

    char *last_dir;
    char *cur_dir;
    char *char_dir;
};

#endif // __MAIN_H__
