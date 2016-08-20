/*
   Copyright (C) 2002/2016 Kai Sterker <kai.sterker@gmail.com>
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
 * @file nls.h
 *
 * @author Kai Sterker
 * @brief National Language Support
 */

#ifndef __NLS_H__
#define __NLS_H__

#include "prefs.h"

#ifndef SWIG
using std::string;
#endif

/** 
 * Provides various services for i18n and l10n.
 */
class nls
{
public:
    /**
     * Initialize national language support.
     * @param myconfig The Engine's configuration 
     */
    static void init (config &myconfig);
    
    /**
     * Set or change the language to use. Setting the language will
     * never fail, but only if there is a binary message catalogue
     * in <gamedir>/po/<country code>LC_MESSAGES/ named <gamename>.mo
     * the desired language will be actually used.
     * @param language The country code of the desired language.
     */
    static void set_language (const string &language);
    
    /**
     * Translate the given string if it's found in the message
     * catalogue. This is just a wrapper around the GNU gettext
     * function for older versions of Python.
     * @param text The text to translate
     * @return either the translated text, or the given text if
     *      no translation was found.
     */
    static const string translate (const string &text);

private:
#ifndef SWIG
	/** current content of the LANGUAGE environment variable */
    static std::string current_lang;
#endif
};

#endif // __NLS_H__
