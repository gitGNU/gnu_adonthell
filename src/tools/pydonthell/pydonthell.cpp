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

#include "game.h"
#include "py_inc.h"

extern "C" {
extern int Py_Main(int, char **);
}

int main(int argc, char * argv[])
{
    game the_game (0, NULL);

    // Init the game engine
    if (!the_game.init ()) 
        return 1;
    return Py_Main(argc,argv);
}
