/*
   $Id$
 
   Copyright (C) 1999/2000/2001 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <cstdio>
#include <string>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <getopt.h>

#include "game.h"
#include "python_class.h"

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
    config myconfig;
    myconfig.read_adonthellrc (); 
    myconfig.gamedir = "."; 
    
    // set the game directory
    if (getopt (argc, argv, "g:") != -1)
    {
        myconfig.gamedir = optarg;
        
        // Python does not like the -g argument, so remove it
        for (int i = 1; i < argc; i++)
            if (strcmp (argv[i], "-g") == 0)
            {
                for (int j = i; j < argc-2; j++)
                    argv[j] = argv[j+2];
                
                argc -= 2;
                break;
            }
    }
    
    if (!game::init (myconfig)) 
        return 1;
    
    // add the working directory to python's path (otherwise, scripts
    // without absolute path cannot be executed)
    string path = dirname (argv[0]);
    if (path[0] != '/') 
    {
        string tmp = getcwd (NULL, 0);
        path = tmp + path;
    }

    // the only way to pass that path to python seems to be the
    // PYTHONPATH environment variable
    string pythonpath = path;
    char *env = getenv ("PYTHONPATH");
    
    if (env)
    {
        pythonpath += ":";
        pythonpath += env;
    }
    
    setenv ("PYTHONPATH", pythonpath.c_str (), 1);
    
    Py_Main (argc, argv);
    game::cleanup (); 
}
