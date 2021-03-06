/*
   (C) Copyright 2000/2001/2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   dialog.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the dialog class.
 * 
 * 
 */
 
#include <algorithm>
#include <iostream>
#include <string.h>

#include "character.h"
#include "dialog.h"
#include "nls.h"
#include "yarg.h"

// Constructor
dialog::dialog (character_base *npc)
{
    npc_portrait_= npc->get_portrait ();
    npc_name_ = npc->get_name ();
    npc_color_ = npc->get_color ();
}

// Destructor
dialog::~dialog ()
{
    clear ();
}

// Prepare the dialogue for execution
bool dialog::init (string fpath, string name, PyObject *args)
{
    // Load and instanciate the dialogue object
    if (!dialogue.create_instance (fpath, name, args))
        return false;

    // Remaining setup tasks
    if (!setup ())
        return false;

    return true;
}

// Misc. initialisation
bool dialog::setup ()
{
    PyObject *list, *s;
    u_int32 i, size;
    
    // Get the text that may loop
    list = dialogue.get_attribute ("loop");
    if (list && PyList_Check (list))
    {
        size = PyList_Size (list);
    
        for (i = 0; i < size; i++)
        {
            s = PyList_GetItem (list, i);
            if (s && PyInt_Check (s)) loop.push_back (PyInt_AsLong (s));
        }
        
        Py_DECREF (list);
    }
    
    // Extract the dialogue's strings
    list = dialogue.get_attribute ("text");
    if (list && PyList_Check (list))
    {
        size = PyList_Size (list);
        strings.resize(size);

        for (i = 1; i < size; i++)
        {
            s = PyList_GetItem (list, i);
            if (s && PyString_Check (s)) strings[i] = python::as_string (s);
            else strings[i] = "*** Error";
        }

        Py_DECREF (list);
    }
    else return false;

    // Init the first answer
    answers.push_back (0);
        
    return true;
}

// Reload a dialogue script that has changed on disk
bool dialog::reload (string fpath, string name, PyObject *args)
{
    // Load and instanciate the dialogue object
    if (!dialogue.reload_instance (fpath, name, args))
        return false;

    // Remaining setup tasks
    if (!setup ())
        return false;

    return true;
}

// Clean up
void dialog::clear ()
{
    strings.clear();
}

// iterate over the dialogue text
string dialog::text ()
{
    if (i_text == text_.end ())
    {
        i_text = text_.begin ();
        return "";
    }
    
    return *i_text++;
}

// Gets the index of either the player or npc array
void dialog::run (u_int32 index)
{
    PyObject *arg, *result, *speaker, *speech;
    s_int32 s, answer = answers[index];
    u_int32 stop, size;
    
    // empty previous dialogue text
    text_.clear ();
    answers.clear ();
    
    // end of dialogue
    if (answer == -1)
        return;
    
    // Mark the Player text as used unless loops allowed
    if (find (loop.begin (), loop.end (), answer) == loop.end ())
        used.push_back (answer);
        
    do
    {
        // Execute the next part of the dialogue
        arg = Py_BuildValue ("(i)", answer);
        
         // run next part of dialogue
        dialogue.run (arg);
#ifdef PY_DEBUG
        python::show_traceback ();
#endif
        Py_XDECREF (arg);
    
        // Now fill in the NPC's and Player's responses:
        // 1. Get the neccesary attributes of the dialogue class
        speaker = dialogue.get_attribute ("speaker");
        speech = dialogue.get_attribute ("speech");

        // 2. Search the NPC part for used text
        for (int i = 0; i < PyList_Size (speech); i++)
        {
            s = PyInt_AsLong (PyList_GetItem (speech, i));

            // Remove text that was already used and isn't allowed to loop
            if (find (used.begin (), used.end (), s) != used.end ())
            {
                PySequence_DelItem (speaker, i);
                PySequence_DelItem (speech, i--);
            }
        }

        // check if some text is left at all
        size = PyList_Size (speech);
        if (size == 0) 
        {
            i_text = text_.begin ();
            return;
        }
                
        // prepare the random number generator        
        yarg::range (0, size - 1);

        // check type of speaker
        if (PyList_GetItem (speaker, 0) != Py_None)
        {
            // got NPC text, so let the engine decide
            int rnd = yarg::get ();
            
            // get the text
            answer = PyInt_AsLong (PyList_GetItem (speech, rnd));
            text_.push_back (scan_string (nls::translate (strings[answer])));

            // get the NPC color, portrait and name
            string npc = python::as_string (PyList_GetItem (speaker, rnd));
            if (!npc.empty())
            {
                if (npc == "Narrator") npc_color_ = 0;
                else
                {
                    // set color and portrait of the NPC
                    character_base *mychar = data::characters[npc];

                    npc_color_ = mychar->get_color ();
                    npc_portrait_ = mychar->get_portrait ();
                    npc_name_ = npc;
                }
            }
            
            // check whether we shall continue or not
            arg = Py_BuildValue ("(i)", answer);
            result = dialogue.call_method_ret ("stop", arg);
            stop = PyInt_AsLong (result);
            Py_XDECREF (result);
            Py_XDECREF (arg);
            
            // Mark the NPC text as used unless loops allowed
            if (find (loop.begin (), loop.end (), answer) == loop.end ())
                used.push_back (answer);
            
            answers.push_back (answer);
        }
        else
        {
            // got Player text, so let the player decide
            for (u_int32 i = 0; i < size; i++)
            {
                // simply add all text to let the player select an answer
                answer = PyInt_AsLong (PyList_GetItem (speech, i));
                text_.push_back (scan_string (nls::translate (strings[answer])));
                answers.push_back (answer);
            }
            
            // let the player make his decision
            stop = true;
        }
    
        // cleanup
        Py_XDECREF (speaker);
        Py_XDECREF (speech);
    }
    while (!stop);            

    // init the iterator for dialogue text retrieval
    i_text = text_.begin ();
}

// execute embedded functions and replace shortcuts
// yeah, the c string library hurts, but at least it's fast ;)
string dialog::scan_string (const string & s)
{
    u_int32 begin, end, len;
    PyObject *result;
    const char *start;
    char *tmp, *mid, *str = NULL;
    character *the_player = data::the_player;
    string newstr (s); 

    // replace $... shortcuts
    while (1)
    {
        // check wether the string contains shortcut code at all
        start = strchr (newstr.c_str (), '$');
        if (start == NULL) break;

        // replace "$name"
        if (strncmp (start, "$name", 5) == 0)
        {
            begin = newstr.length () - strlen (start);
            string t (newstr, 0, begin);
            t += the_player->get_name ();
            t += (start+5);
            
            newstr = t;
            continue;
        }

        // replace "$fm"
        if (strncmp (start, "$fm", 3) == 0)
        {
            // extract the "$fm{.../...} part
            end = strcspn (start, "}");
            str = new char[end];
            str[end-1] = 0;        
            strncpy (str, start+3, end);

            if (the_player->storage::get_val ("gender") == FEMALE)
                mid = get_substr (str, "{", "/");
            else
                mid = get_substr (str, "/", "}");

            begin = newstr.length () - strlen(start);
            tmp = new char[newstr.length () - end + strlen (mid)];
            strncpy (tmp, newstr.c_str (), begin);
            tmp[begin] = 0;
            strcat (tmp, mid);
            strcat (tmp, start+end+1);
            
            delete[] str;
            delete[] mid;
            newstr = tmp;

            continue;
        }

        // Error!
        cout << "\n*** Error, unknown macro " << start << flush;
        newstr[newstr.length () - strlen (start)] = ' ';
    }
    
    // execute python functions
    string repl;
    while (1)
    {
        // check whether the string contains python code at all
        start = strchr (newstr.c_str (), '{');
        if (start == NULL) break;

        end = strcspn (start, "}");
        repl = "";

        str = new char[end];
        str[end-1] = 0;        

        // extract the code without the brackets
        strncpy (str, start+1, end-1);

        // run the string
        result = PyObject_CallMethod (dialogue.get_instance (false), str, NULL);

        if (result)
        {
            if (PyString_Check (result))
            	repl = string(nls::translate (python::as_string(result)));
        }
#ifdef PY_DEBUG
        else
        {
        	python::show_traceback();
        }
#endif
        
        // Replace existing with new, changed string
        // 1. Calculate string's length
        len = newstr.length ();
        begin = len - strlen (start);
        tmp = new char[(repl.length()) + len - strlen(str)];

        // 2. Merge prefix, resulting string and postfix into new string
        strncpy (tmp, newstr.c_str (), begin);
        tmp[begin] = 0;
        if (!repl.empty()) strcat (tmp, repl.c_str());
        strcat (tmp, start+end+1);

        // 3. Exchange strings
        newstr = tmp;

        // Cleanup
        Py_XDECREF (result);
        delete[] str;
        delete[] tmp;
    }

    return newstr;
}

char *dialog::get_substr (const char* string, const char* begin, const char* end)
{
    u_int32 b, e;
    b = strcspn (string, begin) + 1;
    e = strcspn (string, end) - b;

    char *result = new char[e+1];
    strncpy (result, string+b, e);
    result[e] = 0;

    return result;
}
