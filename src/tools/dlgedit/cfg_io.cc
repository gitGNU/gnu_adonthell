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
 * @file cfg_io.cc
 *
 * @author Kai Sterker
 * @brief I/O Routines for dlgedit configuration file
 */

#include <stdio.h>
#include "cfg_io.h"
#include "dlg_types.h"

// The config file opened by the lexical scanner
extern FILE* loadcfgin;

extern int parse_cfgfile (std::string&, int&);

// ctor; load config
CfgIO::CfgIO ()
{
    Dlgeditrc = std::string (getenv ("HOME")) + "/.adonthell/dlgeditrc";
       
    // loadcfgin is declared in lex.loadcfg.cc
    loadcfgin = fopen (Dlgeditrc.c_str (), "r");

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
            case LOAD_FILE:
            {
                if (parse_cfgfile (s, n) == LOAD_STR) Data.addFile (s);               
                break;
            }
            
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
