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

    void write_character_source ();
    void read_character_source (gchar*);
    void load_list_defaults (GtkCList *, gchar *, gchar *);
    void colorify_list (GtkCList *);
    void set_option (GtkOptionMenu *, gchar *);
    gchar *get_option (GtkOptionMenu *);
    gchar *get_script ();

    GtkWidget *app;
    GtkWidget *name_entry;
    GtkWidget *race_choice;
    GtkWidget *gender_choice;
    GtkWidget *attrib_entry;
    GtkWidget *val_entry;
    GtkWidget *attribute_list;
    GtkWidget *notebook;
    int attribute_list_sel;

    GtkWidget *event_choice;
    GtkWidget *event_list;
    int event_list_sel;

    GtkWidget *dlg_entry;
    GtkWidget *scl_entry;

    gchar *last_dir;
    gchar *char_dir;
    gchar *cur_dir;

    unsigned int npc_color;

    static gchar *events[5];    // = { "Enter", "Leave", "Pickup", "Drop", "Kill" };
    static gchar *races[4];     // = { "Dwarf", "Elf", "Half-Elf", "Human" };
    static gchar *gender[2];    // = { "Female", "Male" }; 
};

class event_wnd
{
  public:
    event_wnd (main_wnd *, gchar *);

    GtkWidget *script_entry;
    GtkWidget *condition_label;
    GtkWidget *condition_entry;
    GtkWidget *condition_list;
    int condition_list_sel;

    gchar *script;
    gchar *params;

    main_wnd *main;

    bool ok;
};

#endif // __MAIN_H__
