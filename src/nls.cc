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
 * @file nls.cc
 *
 * @author Kai Sterker
 * @brief National Language Support
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <locale.h>
#include "gettext.h"
#include "nls.h"

// Initialize NLS
void nls::init (config &myconfig)
{
#if ENABLE_NLS
    // if no language specified in the config file, determine
    // the locale from the environment variables 
    if (myconfig.language == "")
        setlocale (LC_MESSAGES, "");
    // otherwise overwrite any environment variables
    else 
        set_language (myconfig.language);
    
    // open the message catalogue
    std::string location = myconfig.gamedir + "/locale";
    bindtextdomain (myconfig.game_name.c_str (), location.c_str ());
    textdomain (myconfig.game_name.c_str ());
#endif
}

// Set the language to use
void nls::set_language (const string &language)
{    
#if ENABLE_NLS
#if !defined (WIN32) && !defined (__BEOS__)
    setenv ("LANGUAGE", language.c_str (), 1);
#endif
    {
        // tell gettext that the language has changed
        extern int _nl_msg_cat_cntr;
        ++_nl_msg_cat_cntr;
    }
    setlocale (LC_MESSAGES, language.c_str ());
#endif
}

// Translate some text
const char* nls::translate (const string &text)
{
#if ENABLE_NLS
    return gettext (text.c_str ());
#else
    return text.c_str();
#endif
}
