/*
   $Id$
 
   Copyright (C) 1999 - 2001 The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <unistd.h>

#include "game.h"
#include "py_inc.h"

extern "C" {
extern int Py_Main(int, char **);
}

int main(int argc, char * argv[])
{
    game the_game (0, NULL);

    char *cwd = getcwd (NULL, 0);
    char tmp[256];
    
    // Init the game engine
    if (!the_game.init ()) 
        return 1;

    if (argc > 1 && argv[1][0] != '/') 
    {
        sprintf (tmp, "%s/%s", cwd, argv[1]);
        argv[1] = tmp;
    }

    return Py_Main(argc,argv);
}
