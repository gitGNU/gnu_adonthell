/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file cfg_io.cc
 *
 * @author Kai Sterker
 * @brief I/O Routines for dlgedit configuration file
 */

#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <glib/gstdio.h>
#include "cfg_io.h"
#include "dlg_types.h"

// The config file opened by the lexical scanner
extern FILE* loadcfgin;

extern int parse_cfgfile (std::string&, int&);

// ctor; load config
CfgIO::CfgIO ()
{
#if defined(__APPLE__)
    // OSX
	Dlgeditrc = std::string (getenv ("HOME")) + "/Library/Application Support/Adonthell/dlgeditrc";
#elif defined (WIN32)
    // Windows
    char *appDataDir = getenv ("APPDATA");
    if (appDataDir != NULL && strlen (appDataDir) > 0)
    	Dlgeditrc = std::string (getenv("APPDATA")) + "/Adonthell/dlgeditrc";
    else
    	Dlgeditrc = "./dlgeditrc";
#else
    // Unix
    char *xdgDir = getenv ("XDG_CONFIG_HOME");
    if (xdgDir != NULL && strlen (xdgDir) > 0)
    	Dlgeditrc = std::string (xdgDir) + "/adonthell/dlgeditrc";
    else
    	Dlgeditrc = std::string (getenv ("HOME")) + "/.config/adonthell/dlgeditrc";
#endif
    
    // loadcfgin is declared in lex.loadcfg.cc
    loadcfgin = g_fopen (Dlgeditrc.c_str (), "rb");

    // open succeeded -> read configuration
    if (loadcfgin)
    {
        load ();

        // cleanup
        fclose (loadcfgin);
    }
}

// dtor; save config
CfgIO::~CfgIO ()
{
    save ();    
}

// load config
void CfgIO::load ()
{
    int token = 1, n;
    std::string s;
    
    // as long as reading something from file ...
    while (token)
    {
        // get next token
        switch (token = parse_cfgfile (s, n))
        {
            case LOAD_PROJECT:
            {
                if (parse_cfgfile (s, n) == LOAD_STR) Data.addProject (s);
                break;
            }
            
            default: break;
        }
    }
    
    return;
}

// save config
void CfgIO::save ()
{
    // open file for writing
    std::ofstream out (Dlgeditrc.c_str ());
    
    // opening failed for some reasons    
    if (!out) return;
    
    // write header
    out << "# Adonthell Dialogue Editor configuration file\n" << std::endl;
            
    // save data
    Data.save (out);
} 
