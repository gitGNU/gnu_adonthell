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

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <gtk/gtkstyle.h>

class MainFrame;

class debug_dlg
{
public:
    debug_dlg ();
    ~debug_dlg ();
    void init ();
    void update ();

    GtkWidget *quest_tree;
    GtkWidget *char_tree;
    GtkWidget *val_entry;
    GtkWidget *attrib_entry;
    GtkWidget *dlg;
    MainFrame *wnd;

    static int destroy;
};

#endif // __DEBUG_H__