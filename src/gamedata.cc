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


/**
 * @file   gamedata.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the gamedata and data classes.
 * 
 * 
 */ 


#include <iostream> 
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "gamedata.h"
#include "character.h"
#include "quest.h"
#include "mapengine.h"
#include "python.h"

vector<gamedata*> gamedata::saves;       // The list of available savegames
string gamedata::user_data_dir_;         // The user's private adonthell directory
string gamedata::game_data_dir_;         // The adonthell data directory


using namespace std; 


gamedata::gamedata ()
{
}

gamedata::gamedata (string dir, string desc)
{
    directory_ = dir;
    description_ = desc; 
}

gamedata::~gamedata ()
{
}

bool gamedata::get (igzstream& file)
{
    if (!fileops::get_version (file, 2, 2, "save.data")) 
        return false;
    directory_ << file; 
    description_ << file;
    location_ << file;
    time_ << file; 
    return true;
}

void gamedata::put (ogzstream& file)
{
    fileops::put_version (file, 2);
    directory_ >> file;
    description_ >> file;
    location_ >> file;
    time_ >> file; 
}

void gamedata::set_description (string desc)
{
    description_ = desc; 
}

void gamedata::set_directory (string dir)
{
    directory_ = dir;
}



bool gamedata::load (u_int32 pos)
{
    igzstream in;
    
    string filepath;
    character *mynpc;
    quest *myquest;
    
    // First, unload the current game
    unload ();
    
    // Create a player (later: load from file or whatever)
    data::the_player = new character;
    data::the_player->set_name ("Player");
    
    // Add the player to the game objects
    data::characters[data::the_player->get_name().c_str ()] = data::the_player; 
    
    // retrieve character array
    PyObject *chars = PyDict_GetItemString (data::globals, "characters");

    // Make the player available to Python
    PyObject *the_player = python::pass_instance (data::the_player, "character"); 
    PyDict_SetItemString (data::globals, "the_player", the_player);
    
    PyDict_SetItemString (chars, (char *) data::the_player->get_name().c_str (),
                          the_player);
    Py_DECREF (the_player); 

    // try to open character.data
    filepath = saves[pos]->directory ();
    filepath += "/character.data";     
    in.open (filepath);
    
    if (!in.is_open ())
    {
        cerr << "Couldn't open \"" << filepath << "\" - stopping\n" <<  endl; 
        return false;
    }
    
    if (!fileops::get_version (in, 2, 2, filepath)) 
        return false;
    
    // load characters     

    char ctemp;
    while (ctemp << in) 
    {
        mynpc = new character;
        mynpc->character_base::get_state (in);
 
        // Pass character over to Python interpreter
        PyObject *charref = python::pass_instance (mynpc, "character"); 
        PyDict_SetItemString (chars, (char *) mynpc->get_name().c_str (),
                              charref);
        Py_DECREF (charref);
        
        // Make this character available to the engine
        data::characters[mynpc->get_name ().c_str ()] = mynpc;
    }
    in.close (); 
    
    // retrieve quest array
    PyObject *quests = PyDict_GetItemString (data::globals, "quests");

    // try to open quest.data
    filepath = saves[pos]->directory ();
    filepath += "/quest.data"; 
    in.open (filepath); 

    if (!in.is_open ())
    {
        cerr <<  "Couldn't open \"" << filepath << " - stopping\n" << endl; 
        return false;
    }
    
    if (!fileops::get_version (in, 1, 1, filepath))
        return false;
    
    // load quests
    while (ctemp << in) 
    {
        myquest = new quest;
        myquest->load (in);
        
        // Pass quest over to Python interpreter
        PyObject *questref = python::pass_instance (myquest, "quest"); 
        PyDict_SetItemString (quests, (char *) myquest->name.c_str (),
                              questref);
        Py_DECREF (questref); 
        
        // Make this quest available to the engine
        data::quests[myquest->name.c_str ()] = myquest;
    }

    in.close (); 
    
    // Load mapengine state
    filepath = saves[pos]->directory(); 
    filepath += "/mapengine.data";
    in.open (filepath); 
    
    if (!in.is_open ())
    {
        cerr <<  "Couldn't open \"" << filepath << " - stopping\n" << endl; 
        return false;
    }
    
    if (!fileops::get_version (in, 1, 1, filepath))
        return false;
    
    if (!data::map_engine->get_state(in))
    {
        cerr << "Couldn't load \"" << filepath << " - stopping\n" << endl;
        return false;
    }
    
    in.close (); 

    return true; 
}

bool gamedata::save (u_int32 pos, string desc)
{
    gamedata *gdata;
    string filepath;
    char t[10]; 
    ogzstream file; 
    char vnbr; 
    
    // make sure we don't overwrite the default game
    if (pos == 0) return false;
    
    // see whether we're going to save to a new slot
    if (pos >= saves.size ())
    {
        int success = 1;
        
        // make sure we save to an unused directory
        while (success)
        {
            // that's the directory we're going to save to
            sprintf(t,"%03i",pos++);
            filepath = user_data_dir ();
            filepath += "/adonthell-save-";
            filepath += t;
            
            success = mkdir (filepath.c_str(), 0700);

            // prevent infinite loop if we can't write to the directory
            if (pos >= 1000) 
            {
                cerr << "Save failed - seems like you have no write permission in\n"
                     << user_data_dir () << endl; 
                return false;
            }
        }
        
        // we'll need a new gamedata record
        gdata = new gamedata (filepath, desc);
    }
    else
    {
        gdata = saves[pos];
        gdata->set_description (desc);
    }

    // save characters 
    filepath = gdata->directory ();
    filepath += "/character.data"; 
    file.open (filepath); 

    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n"; 
        return false;
    }
    
    fileops::put_version (file, 2);

    dictionnary <character *>::iterator itc; 
    for (itc = data::characters.begin (); itc != data::characters.end (); itc++) 
    {
        // don't save the player
        if (itc->second == (character*) data::the_player) continue;
        
        // tell the character.data loader that another entry follows
        vnbr = 1; 
        vnbr >> file; 
        
        // append the character data
        itc->second->character_base::put_state (file);
    }

    // write EOF
    vnbr = 0; 
    vnbr >> file;
    file.close (); 

    // save quests
    filepath = gdata->directory ();
    filepath += "/quest.data"; 
    file.open (filepath); 
    
    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n";
        return false;
    }

    fileops::put_version (file, 1);

    dictionnary <quest *>::iterator itq;
    for (itq = data::quests.begin (); itq != data::quests.end (); itq++)
    {
        // tell the quest.data loader that another entry follows
        vnbr = 1;
        vnbr >> file; 
        
        // append the character data
        itq->second->save (file);
    }

    // write EOF
    vnbr = 0; 
    vnbr >> file; 
    file.close ();
    
    // Save mapengine state
    filepath = gdata->directory(); 
    filepath += "/mapengine.data";
    file.open (filepath); 
    
    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n"; 
        return false;
    }

    fileops::put_version (file, 1);
    data::map_engine->put_state(file);
    file.close (); 

    // save gamedata
    filepath = gdata->directory ();
    filepath += "/save.data"; 

    file.open (filepath); 
    if (!file.is_open ())
    {
        cerr << "Couldn't create \"" << filepath << "\" - save failed\n"; 
        return false;
    }
    
    gdata->put (file);
    file.close ();
    
    // only now it is safe to add the new record to the array
    if (pos >= saves.size ()) saves.push_back (gdata);
    
    return true;  
}

gamedata* gamedata::next_save ()
{
    static vector<gamedata*>::iterator i = saves.begin ();
    static u_int32 size = saves.size ();
    
    // check whether a new save has been added
    if (size != saves.size ())
    {
        size = saves.size ();
        i = saves.begin ();
    }
    
    // check whether we reached the end of the list
    if (++i == saves.end ())
    {
        i = saves.begin ();
        return NULL;
    }
    
    return *i;
}


bool gamedata::init (string udir, string gdir)
{
    DIR *dir;
    igzstream in;
    struct dirent *dirent;
    struct stat statbuf;
    gamedata *gdata;

    user_data_dir_ = udir; 
    game_data_dir_ = gdir; 
    
    // try to change into data directory
    if (chdir (game_data_dir ().c_str ()))
    {
        cerr << "\nSeems like " << game_data_dir () << " is no valid data directory."; 
        cerr << "\nIf you have installed the Adonthell data files into a different location,";
        cerr << "\nplease make sure to update the $HOME/.adonthell/adonthellrc file\n";
        return false;
    }

    // Add the default savegame used to start a new game to the list of saves
    gdata = new gamedata ( ".", "Start New Game");
    saves.push_back (gdata);

    // Read the user's saved games (if any) - they'll be located in
    // $HOME/.adonthell/ and called adonthell-save-xxx
    if ((dir = opendir (user_data_dir ().c_str ())) != NULL)
    {
        while ((dirent = readdir (dir)) != NULL)
        {
            string filepath = user_data_dir () + "/";
            filepath += dirent->d_name; 
 
            if (stat (filepath.c_str (), &statbuf) != -1 && S_ISDIR (statbuf.st_mode) && 
                strncmp ("adonthell-save-", dirent->d_name, 15) == 0)
            {
                // found a (possibly) valid saved game directory
                filepath += "/save.data"; 
                // Now try to read the saved game's data record
                in.open (filepath); 
                
                if (in.is_open ())
                {
                    // restore the pathname
                    filepath = user_data_dir ();
                    filepath += "/";
                    filepath += dirent->d_name;
                    
                    gdata = new gamedata;
                    if (gdata->get (in))
                    {
                        gdata->set_directory (filepath);
                        saves.push_back (gdata);
                    }
                    else delete gdata;
                    
                    in.close (); 
                }
            }
        } 
        closedir (dir);
    }    
    return true; 
}

void gamedata::cleanup () 
{ 
    for (vector<gamedata*>::iterator i = saves.begin (); i != saves.end (); i++)
        delete *i;
    saves.clear (); 
    unload (); 
}

void gamedata::unload () 
{
    // delete all characters
    dictionnary <character *>::iterator itc; 
    for (itc = data::characters.begin (); itc != data::characters.end (); itc++) 
    {
        itc->second->remove_from_map (); 
        delete itc->second;
    }
    data::characters.clear (); 
    
    // delete all quests
    dictionnary <quest *>::iterator itq; 
    for (itq = data::quests.begin (); itq != data::quests.end (); itq++) 
        delete itq->second;
    data::quests.clear (); 
    
    // the main character was deleted with the other characters already
    data::the_player = NULL;
}
