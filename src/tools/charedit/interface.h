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

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "main.h"

GtkWidget *create_main_wnd (main_wnd &);
GtkWidget *create_event_wnd (event_wnd *, gchar *);
GtkWidget *create_fileselection (GString *, bool);
GtkWidget *create_warning (gchar *);

#endif // __INTERFACE_H__
