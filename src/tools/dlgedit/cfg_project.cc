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
 * @file cfg_project.cc
 *
 * @author Kai Sterker
 * @brief Project specific configuration data
 */

#include "cfg_project.h"
#include "dlg_types.h"

extern int parse_cfgfile (std::string&, int&);

// ctor
CfgProject::CfgProject (std::string &name)
{
    Name = name;
    BaseDir = "";
}

// load project data
bool CfgProject::load ()
{
    int token = 1, n;
    std::string s;
    
    // as long as reading something from file ...
    while (token)
    {
        // get next token
        switch (token = parse_cfgfile (s, n))
        {
            case LOAD_END: return true;
            
            case LOAD_BASE_DIR:
            {
                if (parse_cfgfile (s, n) == LOAD_STR) BaseDir = s;
                break;
            }
            
            default: break;
        }
    }
    
    return false;
}

// save project data
void CfgProject::save (ofstream &out)
{
    out << "Project [" << Name << "]\n"
        << "  BaseDir [" << BaseDir << "]\n"
        << "End" << endl;
}
