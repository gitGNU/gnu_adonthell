/*
   $Id$
   
   Copyright (C) 1999/2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
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
#include <iostream.h>
#include <locale.h>
#include "gettext.h"
#include "game.h"
#include "dlg_cmdline.h"
#include "dlg_compiler.h"
#include "gui_dlgedit.h"

int main (int argc, char *argv[])
{
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
        
        while (DlgCmdline::sources < argc)
        {
            dialogue = argv[DlgCmdline::sources++];
            
            // check whether the file is a valid dialoge
            if (!GuiDlgedit::checkDialogue (dialogue))
            {
                cout << "Loading of '" << dialogue << "' failed\n";
                continue;
            }
            
            module = new DlgModule (dialogue + "-1", "");
            
            // try to load from file
            if (!module->load ())
            {
                cout << "Loading of '" << dialogue << "' failed\n";
            }
            else
            {            
                cout << "Compiling '" << dialogue << "' ...\n";
            
                // try to compile the dialogue
                DlgCompiler compiler (module);
    
                // compile
                compiler.run ();
            }
            
            delete module;
        }
    }
    
    // good bye
    return 0;
}
