/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.#include <sys/types.h>
*/

#include <sys/stat.h>
#include <glib.h>
#include <dirent.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>

#include "../../character.h"
#include "../../prefs.h"
#include "../../event.h"
#include "../../game.h"

PyObject *game::globals;
objects game::characters;

class dialog_engine
{
public:
    dialog_engine (const char*);
    void run ();
};

dialog_engine::dialog_engine (const char *unused) { }
void dialog_engine::run () { }

// Dump any error information to stderr (same as in ../../py_inc.cpp)
void show_traceback(void)
{
	if ( PyErr_Occurred() )
		PyErr_Print();
}

PyObject *pass_instance (void *instance, const char *class_name)
{
}

void process_character (char *input, FILE *output)
{
    ifstream in (input);
    gchar str[256], **vals;
    int mode = 0;
    
    if (!in)
    {
        cout << " failed\n";
        return;
    }

    npc mynpc;
    
    
    // read character data from source file
    while (!in.eof ())
    {
        in.getline (str, 255);
        g_strstrip (str);

        // ignore comments or blank lines
        if (str != NULL && str[0] != '#' && str[0] != '\0')
        {
            if (strcmp (str, "basic:") == 0)
            {
                mode = 1;
                continue;
            }
            if (strcmp (str, "attributes:") == 0)
            {
                mode = 2;
                continue;
            }
            if (strcmp (str, "events:") == 0)
            {
                mode = 3;
                continue;
            }
            if (strcmp (str, "actions:") == 0)
            {
                mode = 4;
                continue;
            }

            vals = g_strsplit (str, "=", 1);
            g_strstrip (vals[0]);
            g_strstrip (vals[1]);

            switch (mode)
            {
                case 1:
                {
                    if (strcmp (vals[0], "name") == 0) mynpc.name = strdup (vals[1]);
                    if (strcmp (vals[0], "posx") == 0) mynpc.posx = atoi (vals[1]);
                    if (strcmp (vals[0], "posy") == 0) mynpc.posy = atoi (vals[1]);
                    if (strcmp (vals[0], "race") == 0)
                    {
                        if (strcmp (vals[1], "Elf") == 0) mynpc.set ("race", ELF);
                        if (strcmp (vals[1], "Half-Elf") == 0) mynpc.set ("race", HALFELF);
                        if (strcmp (vals[1], "Human") == 0) mynpc.set ("race", HUMAN);
                        if (strcmp (vals[1], "Dwarf") == 0) mynpc.set ("race", DWARF);
                    }
                    if (strcmp (vals[0], "gender") == 0)
                    {
                        if (strcmp (vals[1], "Male") == 0) mynpc.set ("gender", MALE);
                        if (strcmp (vals[1], "Female") == 0) mynpc.set ("gender", FEMALE);
                    }
                    break;
                }
                case 2:
                {
                    mynpc.set (strdup (vals[0]), atoi (vals[1]));

                    break;
                }
                case 3:
                {
                    if (strcmp (vals[0], "type") == 0)
                    {
                        // gtk_clist_append (GTK_CLIST (event_list), dummy);
                        // gtk_clist_set_text (GTK_CLIST (event_list), i, 0, vals[1]);
                    }
                    if (strcmp (vals[0], "script") == 0);
                        // gtk_clist_set_text (GTK_CLIST (event_list), i, 1, vals[1]);
                    if (strcmp (vals[0], "parameters") == 0);
                        // gtk_clist_set_text (GTK_CLIST (event_list), i++, 2, vals[1]);

                    break;
                }
                case 4:
                {
                    if (strcmp (vals[0], "dialogue") == 0) mynpc.set_dialogue (vals[1]);
                    if (strcmp (vals[0], "schedule") == 0) mynpc.set_schedule (vals[1]);
       
                    break;
                }
            }

            g_strfreev (vals);
        }
    }

    fputc (1, output);
    mynpc.save (output);
    cout << " done\n";
}

int main (int argc, char* argv[])
{
	struct dirent *dirent;
	struct stat statbuf;
    config myconfig ("");
	char *path = NULL, *cwd = NULL;
    FILE *outfile;
	DIR *dir;

    if (argc < 2 || argc > 3)
    {
        cout << "\nusage: " << argv[0] << " <input directory> [<output file>]\n\n";
        return 1;
    }

    if (argc == 2) outfile = fopen ("character.data", "w");
    else outfile = fopen (argv[2], "w");

    if (!outfile)
    {
        cout << "\ncannot open \"" << (argc == 2 ? argv[2] : "character.data") 
             << "\" for writing.\n\n";
        return 1;
    }

    // get absolute pathname
    chdir (argv[1]);
    cwd = getcwd (cwd, 0);

    // try to read adonthellrc
    if (!myconfig.read_adonthellrc ()) return 1;

    // We need Python for loading character schedules
	Py_Initialize();
  
    // process all ".character" files in the given directory:
    if ((dir = opendir (cwd)) != NULL)
    {
        // try to change into data directory
        if (chdir (myconfig.datadir.c_str ()))
        {
            cout << "\nSeems like %s is no valid data directory." << myconfig.datadir.c_str ()
                 << "\nIf you have installed the Adonthell data files into a different location,"
                 << "\nplease make sure to update the $HOME/.adonthell/adonthellrc file\n" << flush;
            return 1;
        }  

        // grab the next file
        while ((dirent = readdir (dir)) != NULL)
        {
            if (path != NULL) delete path;

            path = new char[strlen (cwd) + strlen (dirent->d_name) + 2];
            strcpy (path, cwd);
            strcat (path, "/");
            strcat (path, dirent->d_name);
            
            // look if we get a file
			if (stat (path, &statbuf) != -1 && S_ISREG(statbuf.st_mode))
                // make sure none of strcmp()'s arguments are NULL
                if (strrchr (path, '.') != NULL)
                    // do we have a ",character" file at all? 
                    if (strcmp ( ".character", strrchr (path, '.')) == 0)
                    {
                        cout << "Adding " << strrchr (path, '/')+1 << " ..." << flush;
                        process_character (path, outfile);
                    }
        }
    }
    else
    {
        cout << "\nCannot open directory \"" << cwd << "\"\n\n";
        return 1;
    }

    fputc (0, outfile);
    fclose (outfile);
    
	Py_Finalize();
    return 0;
}