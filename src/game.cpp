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

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "image.h"
#include "input.h"
#include "prefs.h"
#include "character.h"
#include "py_inc.h"
#include "quest.h"
#include "game.h"

#ifdef SDL_MIXER
#include "audio_thread.h"
#include "audio.h"

SDL_Thread *game::audio_thread;
#endif

PyObject *game::globals;
char *game::theme;
game_engine *game::engine;
objects game::characters;
objects game::quests;

void game::init(config &myconfig)
{
    // Pass on resolution/general display data
    screen::init_display (&myconfig);
      
#ifdef SDL_MIXER
    audio_init(myconfig);
    audio_thread = SDL_CreateThread(audio_update, NULL);
    if ( audio_thread == NULL) {
        fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
        fprintf(stderr, "Audio will not be used\n");
    }
#endif  
  
    input::init();

    // Initialise Python
    if (!init_python())
    {
	   // This is unlikely to happen
	   fprintf(stderr, "Couldn't initialise Python - stopping\n");
	   SDL_Quit();
    }
    
    // Set the theme (later: get theme from config file)
    theme = strdup ((myconfig.window_theme + "/").c_str ());

    // load the game (later: continue with the last saved game?!)
    load (myconfig.datadir.c_str (), myconfig.datadir.c_str ());    
}

// Load a game. First try to load all dynamic data from gamedir, then load 
// everything else from the static data directory.
void game::load (const char *gamedir, const char *staticdir)
{
    FILE *in = NULL;
    char filepath[256];
    npc *mynpc;
    quest *myquest;
    
    // Load some modules
    PyObject *m = import_module ("ins_modules");
    Py_INCREF(m);
    
    // Create a player (later: load from file or whatever)
    player *myplayer = new player;
    myplayer->name = "Player";

    // Add the player to the game objects
    characters.set ("the_player", myplayer);

    // Make "myplayer" available to the interpreter 
	globals = PyModule_GetDict(m);
    PyDict_SetItemString (globals, "the_player", pass_instance (myplayer, "player"));

    // create character array
    PyObject *chars = PyDict_New ();
    PyDict_SetItemString (globals, "characters", chars);
    PyDict_SetItemString (chars, "the_player", pass_instance (myplayer, "player"));

    // try to open character.data
    sprintf (filepath, "%s/character.data", gamedir);
    in = fopen (filepath, "r");

    if (!in)
    {
        // try loading from static data-dir then
        sprintf (filepath, "%s/character.data", staticdir);
        in = fopen (filepath, "r");
        
        if (!in)
        {
            fprintf (stderr, "Couldn't open \"character.data\" - stopping\n");
            SDL_Quit ();
            return;
        }
    }

    // load characters     
    while (fgetc (in))
    {
        mynpc = new npc;
        mynpc->load (in);

        // Pass character over to Python interpreter
        PyDict_SetItemString (chars, mynpc->name, pass_instance (mynpc, "npc"));
    }
    
    fclose (in);

    // create quest array
    PyObject *quests = PyDict_New ();
    PyDict_SetItemString (globals, "quests", quests);

    // try to open quest.data
    sprintf (filepath, "%s/quest.data", gamedir);
    in = fopen (filepath, "r");

    if (!in)
    {
        // try loading from static data-dir then
        sprintf (filepath, "%s/quest.data", staticdir);
        in = fopen (filepath, "r");
        
        if (!in)
        {
            fprintf (stderr, "Couldn't open \"quest.data\" - stopping\n");
            SDL_Quit ();
            return;
        }
    }
    
    // load quests
    while (fgetc (in))
    {
        myquest = new quest;
        myquest->load (in);
        
        // Pass quest over to Python interpreter
        PyDict_SetItemString (quests, myquest->name, pass_instance (myquest, "quest"));

        // Make this quest available to the engine
        game::quests.set (myquest->name, myquest);
    }

    fclose (in);
}

// Save all dynamic gamedata to the gamedir
void game::save (const char* gamedir)
{
}

void game::cleanup()
{
#ifdef SDL_MIXER
  if (audio_thread != NULL) {
    SDL_KillThread(audio_thread);
    audio_cleanup();
  }
#endif

  Py_DECREF (globals);
  kill_python();

  SDL_Quit();
}
