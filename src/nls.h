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
    static const char* translate (const string &text);
};

#endif // __NLS_H__
