/*
   $Id$
   
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_cmdline.h
 *
 * @author Kai Sterker
 * @brief Methods to parse the dlgedit commandline.
 */

#ifndef DLG_CMDLINE_H
#define DLG_CMDLINE_H

#include <string>

/**
 * Apart from the above, DlgCmdline stores the various options
 * that can be specified on the command line 
 */
class DlgCmdline
{
public:
    /**
     * The method doing all the work. To be called right after
     * dlgedit is launched.
     * @param argc argument count
     * @param argv argument vector
     * @return <b>false</b> indicates that the program shall quit
     */
    static bool parse (int argc, char* argv[]);

    /**
     * The directory where dlgedit searches for projects.
     */
    static std::string datadir;
    
    /**
     * The project. This is the directory that contains the character-
     * and quest data dlgedit needs to (properly) compile dialogues
     * making use of advanced Python scripting features.
     */
    static std::string project;
    
    /**
     * This is set to <b>true</b> to indicate that only the dialogue
     * compiler should be run on the given sourcefiles, without launching
     * the GUI. After all files are compiled, dlgedit exits.
     */
    static bool compile;
    
    /**
     * The index in the argument vector pointing to the first non-option.
     * With a bit of luck, this is one or more dialogue sources.
     */
    static int sources;
    
private:
    static void help (const std::string &program); 
};

#endif // DLG_CMDLINE_H
