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
    ps_dlg (player*);

    void run ();
    void on_ok (char*, int, int);

    GtkEntry *name;
    GtkRadioButton *race;
    GtkRadioButton *gender;
    GtkWidget *wnd;
    
private:
    player *myplayer;
};

#endif // __PSDLG_H__