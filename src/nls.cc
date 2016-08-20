/*
   Copyright (C) 2002/2003/2016 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
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

std::string nls::current_lang;

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
    const char *domain = myconfig.game_name.c_str ();

    bindtextdomain (domain, location.c_str ());
    textdomain (domain);
    bind_textdomain_codeset(domain, "UTF-8");
#endif
}

// Set the language to use
void nls::set_language (const string &language)
{
#if ENABLE_NLS
    current_lang = language.empty() ? "LANGUAGE" : "LANGUAGE=" + language;
    putenv ((char *) current_lang.c_str ());
    {
        // tell gettext that the language has changed
        extern int _nl_msg_cat_cntr;
        ++_nl_msg_cat_cntr;
    }

	setlocale (LC_ALL,"");
    setlocale (LC_MESSAGES, language.c_str ());
#endif
}

// Translate some text
const string nls::translate (const string &text)
{
#if ENABLE_NLS
    return gettext (text.c_str ());
#else
    return text;
#endif
}
