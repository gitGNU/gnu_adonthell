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

#include <cstdio>
#include <unistd.h>

#include "game.h"
#include "python.h"

using namespace std; 

extern "C" {
extern int Py_Main(int, char **);
}

extern "C"
{
    /** 
     * SWIG init prototype.
     * 
     */
    void initadonthellc (void);
}

int main(int argc, char * argv[])
{
    // Init the game engine
    if (!game::init (argc, argv)) 
        return 1;

    
//     char *cwd = getcwd (NULL, 0);
//     char tmp[256];

    
//     if (argc > 1 && argv[1][0] != '/') 
//     {
//         sprintf (tmp, "%s/%s", cwd, argv[1]);
//         argv[1] = tmp;
//     }

//     Py_Initialize (); 
//     initadonthellc ();
    return Py_Main(argc,argv);
    //     return PyRun_AnyFile (stdin, NULL);
    game::cleanup (); 
}
