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

#ifndef __DBG_INTERFACE_H__
#define __DBG_INTERFACE_H__

#include "debug.h"

GtkWidget *create_debug_wnd (debug_dlg*);
GtkWidget* create_dbg_edit_wnd (debug_dlg*, char*, char*, char*);

#endif