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
 *
 */
class DlgCmdline
{
public:
    static bool parse (int argc, char* argv[]);
    static std::string datadir;
    static bool compile;
    static int sources;
    
private:
    static void help (const std::string &program); 
};

#endif // DLG_CMDLINE_H
