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
#include <zlib.h>
#include <glib.h>
#include <dirent.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>

#include "../../quest.h"
#include "../../fileops.h"

// read the quest source file and append it to the quest data file
void process_character (char *input, ogzstream& output)
{
    ifstream in (input);
    gchar str[256], **vals;
    int mode = 0;
    
    if (!in)
    {
        cout << " failed\n";
        return;
    }

    quest myquest;
        
    // read quest data from source file
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
            if (strcmp (str, "variables:") == 0)
            {
                mode = 2;
                continue;
            }
            
            vals = g_strsplit (str, "=", 1);
            g_strstrip (vals[0]);
            g_strstrip (vals[1]);

            switch (mode)
            {
                // read basic attributes
                case 1:
                {
                    if (strcmp (vals[0], "id") == 0) myquest.name = strdup (vals[1]);

                    break;
                }

                // read further attributes and flags
                case 2:
                {
                    myquest.set (strdup (vals[0]), atoi (vals[1]));

                    break;
                }
            }

            g_strfreev (vals);
        }
    }

    // tell the quest.data loader that another entry follows
    char tc = 1;
    tc >> output; 
//     gzputc (output, 1);

    // append the character data
    myquest.save (output);

    cout << " done\n";
}

int main (int argc, char* argv[])
{
	struct dirent *dirent;
	struct stat statbuf;
	char *path = NULL, *cwd = NULL;
    ogzstream outfile;
	DIR *dir;

    if (argc < 2 || argc > 3)
    {
        cout << "\nusage: " << argv[0] << " <input directory> [<output file>]\n\n";
        return 1;
    }

//     if (argc == 2) outfile = gzopen ("quest.data", "w6");
    if (argc == 2) outfile.open ("quest.data");
//     else outfile = gzopen (argv[2], "w6");
    else outfile.open (argv[2]);

    if (!outfile.is_open ())
    {
        cout << "\ncannot open \"" << (argc == 3 ? argv[2] : "quest.data") 
             << "\" for writing.\n\n";
        return 1;
    }

    fileops::put_version (outfile, 1);

    // get absolute pathname
    chdir (argv[1]);
    cwd = getcwd (cwd, 0);
  
    // process all ".quest" files in the given directory:
    if ((dir = opendir (cwd)) != NULL)
    {
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
                    if (strcmp ( ".quest", strrchr (path, '.')) == 0)
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

    // tell the quest.data loader that the EOF has been reached
    char tc = 0;
    tc >> outfile; 
    outfile.close (); 
//     gzputc (outfile, 0);
//     gzclose (outfile);
    
   return 0;
}
