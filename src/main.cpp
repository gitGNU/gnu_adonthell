/*
   $Id$
 
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "input.h"
#include "game.h"
#include "data.h"
#include "landmap.h"
#include "mapview.h"
#include "mapengine.h"
#include "py_inc.h"

// This is nice but very long! `:/
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_anim.h"
#include "win_container.h"
#include "win_select.h"
#include "win_theme.h"
#include "win_background.h"
#include "data_screen.h"
#include "dialog_engine.h"

int main(int argc, char * argv[])
{
    game the_game (argc, argv);

    // Init the game engine
    if (!the_game.init ()) 
        return 1;
    data::load (0);

    exec_file("init.py");

    return 0;
}
