/*
   $Id$

   Copyright (C) 2001 by Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __DATA_SCREEN_H__
#define __DATA_SCREEN_H__

#include "win_types.h"
#include "win_font.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_container.h"

class win_write;

enum
{
    LOAD_SCREEN = 0,
    SAVE_SCREEN = 1
};

// The gui for loading/saving games
class data_screen
{
public:
    data_screen (int);
    ~data_screen ();
    void draw () { window->draw (); }
    void update ();

private:
    void init ();
    void on_select ();
    void save_preview ();

    win_font *font;
    win_theme *theme;
    win_write *entry;
    win_select *text_list;
    win_select *image_list;
    win_container *window;

    image *shot;

    int mode;
};

#endif // __DATA_SCREEN_H__