/*
   Copyright (C) 1999/2000/2001/2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
*/

/** 
 * @file main.cc
 *
 * @author Kai Sterker
 * @brief The main function of dlgedit.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <iostream>
#include <locale.h>
#include "game.h"
#include "gettext.h"
#include "cfg_io.h"
#include "dlg_cmdline.h"
#include "dlg_compiler.h"
#include "gui_dlgedit.h"

int main (int argc, char *argv[])
{
    // dlgedit configuration
    CfgIO config;
    
    // Init GTK+
    gtk_init (&argc, &argv);
    
    // parse command line
    if (!DlgCmdline::parse (argc, argv))
        return 1;

    // Init i18n
    setlocale (LC_ALL, "");
  
    // init game directory
    game::init (DlgCmdline::datadir);

    // start in gui mode
    if (!DlgCmdline::compile)
    {
        // Create the User Interface
        GuiDlgedit dlgedit;
        
        // are there any sources given?
        if (DlgCmdline::sources >= argc)
        {
            // Nope -> create a new dialogue
            dlgedit.newDialogue ();
        }
        else
        {
            // Yep -> load all of them
            while (DlgCmdline::sources < argc)
                dlgedit.loadDialogue (argv[DlgCmdline::sources++]);
        }
        
        // Start the main loop
        gtk_main ();
    }
    
    // just compile what we're given and exit
    else
    {
        DlgModule *module;
        std::string dialogue;
        std::string directory = g_get_current_dir();
        
        while (DlgCmdline::sources < argc)
        {
            dialogue = argv[DlgCmdline::sources++];
            
            // make sure that file has an absolute path
            std::string file = ((dialogue[0] == '/' || dialogue[1] == ':') ? dialogue : directory + std::string ("/") + dialogue);
            gchar *fname = g_path_get_basename (file.c_str ());

            // check whether the file is a valid dialogue
            if (!GuiDlgedit::checkDialogue (dialogue))
            {
                std::cout << "Loading of '" << dialogue << "' failed\n";
                g_free(fname);
                continue;
            }

            // get the name to use for the dialogue
            std::string filename (fname);

            // remove file extension
            unsigned long pos = filename.rfind (FILE_EXT);
            if (pos != filename.npos) filename.erase (pos);

            module = new DlgModule (g_path_get_dirname (file.c_str ()), filename, "-1", "");
            
            // try to load from file
            if (!module->load ())
            {
                std::cout << "Loading of '" << dialogue << "' failed\n";
            }
            else
            {            
                std::cout << "Compiling '" << dialogue << "' ...\n";
            
                // try to compile the dialogue
                DlgCompiler compiler (module);
    
                // compile
                compiler.run ();
            }
            
            g_free(fname);
            delete module;
        }
    }
    
    // good bye
    return 0;
}
