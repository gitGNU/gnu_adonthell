/*
   $Id$

   (C) Copyright 2000/2001/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

/**
 * @file   dialog.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the dialog class.
 * 
 * 
 */
 
#include <algorithm>
#include <iostream>
#include <string>

#include "character.h"
#include "dialog.h"
#include "nls.h"
#include "yarg.h"

// Constructor
dialog::dialog (character_base *npc)
{
    strings = NULL;
    npc_portrait_= npc->get_portrait ();
    npc_name_ = npc->get_name ();
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
    // Extract the dialogue's strings
    PyObject *list = dialogue.get_attribute ("text");
    if (!list || !PyList_Check (list)) return false;

    PyObject *s;
    u_int32 i, index = PyList_Size (list);

    strings = new char*[index];

    for (i = 1; i < index; i++)
    {
        s = PyList_GetItem (list, i);
        if (s) strings[i] = PyString_AsString (s);
        else strings[i] = "*** Error";
    }

    Py_DECREF (list);

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
    if (strings) delete[] strings;
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
    
    // Mark the Player's text (if any) as used unless loops allowed
    if (index > 0)
    {
        used.push_back (answer);
    }
    
    do
    {
        // Execute the next part of the dialogue
        arg = Py_BuildValue ("(i)", answer);
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
        if (size == 0) return;

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
            char *npc = PyString_AsString (PyList_GetItem (speaker, rnd));
            if (npc != NULL)
            {
                if (strcmp ("Narrator", npc) == 0) npc_color_ = 0;
                else
                {
                    // set color and portrait of the NPC
                    character_base *mychar = data::characters[npc];

                    npc_color_ = mychar->get_color ();
                    npc_portrait_ = mychar->get_portrait ();
                    npc_name_ = npc;
                }
            }
            
            cout << "NPC: " << npc << " says \"" << text_.back () << "\n\""
                 << flush;
                        
            // check whether we shall continue or not
            arg = Py_BuildValue ("(i)", answer);
            result = dialogue.call_method_ret ("stop", arg);
            stop = PyInt_AsLong (result);
            Py_XDECREF (result);
            Py_XDECREF (arg);
            
            // make sure this NPC text can't be used any more
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
char* dialog::scan_string (const char *s)
{
    u_int32 begin, end, len;
    PyObject *result;
    char *start, *mid, *string = NULL;
    char *tmp, *newstr = new char[strlen (s)+1];
    character *the_player = data::the_player;
    strcpy (newstr, s); 

    // replace $... shortcuts
    while (1)
    {
        // check wether the string contains shortcut code at all
        start = strchr (newstr, '$');
        if (start == NULL) break;

        // replace "$name"
        if (strncmp (start, "$name", 5) == 0)
        {
            begin = strlen (newstr) - strlen (start);
            tmp = new char[strlen (newstr) - 4 + strlen (the_player->get_name().c_str ())];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            strcat (tmp, the_player->get_name().c_str ());
            strcat (tmp, start+5);
            delete[] newstr;
            newstr = tmp;

            continue;
        }

        // replace "$fm"
        if (strncmp (start, "$fm", 3) == 0)
        {
            // extract the "$fm{.../...} part
            end = strcspn (start, "}");
            string = new char[end];
            string[end-1] = 0;        
            strncpy (string, start+3, end);

            if (the_player->storage::get_val ("gender") == FEMALE)
                mid = get_substr (string, "{", "/");
            else
                mid = get_substr (string, "/", "}");

            begin = strlen(newstr) - strlen(start);
            tmp = new char[strlen(newstr) - end + strlen (mid)];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            strcat (tmp, mid);
            strcat (tmp, start+end+1);
            
            delete[] string;
            delete[] newstr;
            delete[] mid;
            newstr = tmp;

            continue;
        }

        // Error!
        cout << "\n*** Error, unknown macro " << start << flush;
        start[0] = ' ';
    }
    
    // execute python functions
    while (1)
    {
        // check wether the string contains python code at all
        start = strchr (newstr, '{');
        if (start == NULL) break;

        end = strcspn (start, "}");

        string = new char[end];
        string[end-1] = 0;        

        // extract the code without the brackets
        strncpy (string, start+1, end-1);

        // run the string
        result = PyObject_CallMethod (dialogue.get_instance (), string, NULL);
#ifdef PY_DEBUG
        python::show_traceback ();
#endif
        mid = NULL;

        if (result)
            if (PyString_Check (result))
                mid = (char*) nls::translate (PyString_AS_STRING (result));    
        
        // Replace existing with new, changed string
        // 1. Calculate string's length
        len = strlen (newstr);
        begin = len - strlen (start);
        tmp = new char[(mid ? strlen(mid) : 0) + len - strlen(string)];

        // 2. Merge prefix, resulting string and postfix into new string
        strncpy (tmp, newstr, begin);
        tmp[begin] = 0;
        if (mid) strcat (tmp, mid);
        strcat (tmp, start+end+1);

        // 3. Exchange strings
        delete[] newstr;
        newstr = tmp;

        // Cleanup
        Py_XDECREF (result);
        delete[] string;
    }

    return newstr;
}

char *dialog::get_substr (const char* string, char* begin, char* end)
{
    u_int32 b, e;
    b = strcspn (string, begin) + 1;
    e = strcspn (string, end) - b;

    char *result = new char[e+1];
    strncpy (result, string+b, e);
    result[e] = 0;

    return result;
}
