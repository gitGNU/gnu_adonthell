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

#ifdef MEMORY_LEAKS
#include <mcheck.h>
#endif

#include "game.h"
#include "data.h"
#include "py_inc.h"

int main(int argc, char * argv[])
{
#ifdef MEMORY_LEAKS
    mtrace ();
#endif

    game the_game (argc, argv);

    // Init the game engine
    if (!the_game.init ()) 
        return 1;

    if (!data::load (0))
        return 2;

    exec_file("init.py");
    show_traceback ();
    return 0;
}
