/*
   $Id$

   (C) Copyright 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <string.h>
#include <algorithm>

#include "character.h"
#include "mapcharacter.h"
#include "win_font.h"
#include "game.h"
#include "py_inc.h"
#include "yarg.h"
#include "dialog.h"


// Load and instanciate the dialogue object
bool dialog::init (char *fpath, char *name)
{
	PyObject *module;
	PyObject *classobj;

	// First, test if the module has already been imported

	// Seems not, so import
	module = import_module(fpath);

	if (!module)
		return false;

    module = PyImport_ReloadModule (module);

	// Extract the class from the dialogue module
	classobj = PyObject_GetAttrString(module, name);

	if (!classobj)
		return false;

	Py_DECREF(module);

	// Instantiate! Will we ever need to pass args to class
	// constructor here?
	instance = PyObject_CallObject(classobj, NULL);

	Py_DECREF(classobj);

    // extract the dialogue's strings
    extract_strings ();

    // Init the first answer
    answers.push_back(0);

    text = NULL;

	return true;
}

// extract the dialogue's strings
void dialog::extract_strings ()
{
    PyObject *list = PyObject_GetAttrString (instance, "strings");
    PyObject *s;
    u_int32 i, index = PyList_Size (list);

    strings = new char*[index];

    for (i = 0; i < index; i++)
    {
        s = PyList_GetItem (list, i);
        if (s) strings[i] = PyString_AsString (s);
    }
}

dialog::dialog ()
{
    instance = NULL;
    strings = NULL;
}

dialog::~dialog ()
{
    Py_XDECREF (instance);
    if (strings) delete strings;
}

// Gets the index of either the player or npc array
void dialog::run (u_int32 index)
{
    u_int32 nsz, psz, i, j = 0, k = 0, l = 1;
    yarg randgen;
    s_int32 s;
    
    // Is it sufficient to get those objects only once???
    PyObject *npc, *player, *cont;

    // Execute the next part of the dialogue
    PyObject_CallMethod (instance, "run", "i", answers[index]);
#ifdef _DEBUG_
    show_traceback ();
    cout << flush;
#endif // _DEBUG_

    // Mark the Player's text (if any) as used unless loops allowed
    if (index != 0)
    {
        s = choices[index-1];
        if (!PySequence_In (PyObject_GetAttrString (instance, "loop"), PyInt_FromLong (s)))
            used.push_back (s);
    }
    
    // Empty helper arrays
    answers.clear ();
    choices.clear ();

    // Now fill in the NPC's and Player's responses:
    // 1. Get the neccesary attributes of the dialogue class
    npc = PyObject_GetAttrString (instance, "npc");
    player = PyObject_GetAttrString (instance, "player");
    cont = PyObject_GetAttrString (instance, "cont");

    // 2. Search the NPC part for used text
    for (i = 0; (int)i < PyList_Size (npc); i++)
    {
        s = PyInt_AsLong (PyList_GetItem (npc, i));

        // Remove NPC text that was already used and isn't allowed to loop
        if (find (used.begin (), used.end (), s) != used.end ())
            PySequence_DelItem (npc, i--);
    }

    // 3. (Re)Init dialog::text
    if (text)
    {
        for (i = 0; i < text_size; i++) delete text[i];
        delete text;
    }
    
    nsz = PyList_Size (npc);
    psz = PyList_Size (player);

    if (nsz != 0)
    {
        text = new char*[nsz+psz];
    
        // 4. Randomly chose between possible NPC replies
        randgen.init (" ", 0, nsz-1);
    }
    else 
    {
        // End of dialogue
        text = NULL;
        return;
    }
    
    randgen.randomize ();
    i = randgen.get (5);

    // The first value of text is the NPC Part
    s = PyInt_AsLong (PyList_GetItem (npc, i));

    // scan the string for { python code }
    text[0] = scan_string (strings[s]);
    answers.push_back (0);

    // Mark the NPC text as used unless it's allowed to loop
    if (!PySequence_In (PyObject_GetAttrString (instance, "loop"), PyInt_FromLong (s)))
        used.push_back (s);

    // 5. Extract the matching player strings
    while (j <= i)
    {
        s = PyInt_AsLong (PyList_GetItem (player, k));
        
        if (s == -1) j++;
        
        // These are the strings belonging to the chosen NPC text
        if (j == i && s != -1)
        {
            // Only display unused text
            if (find (used.begin (), used.end (), s) == used.end ())
            {
                // add string to current text list
                text[l++] = scan_string (strings[s]);
                
                // Remember Player's possible replies to avoid loops
                choices.push_back (s);               
                answers.push_back (PyInt_AsLong (PyList_GetItem (cont, k+1)));
            }
        }
        
        k++;
    }

    // Insert the target of the NPC text
    answers[0] = PyInt_AsLong (PyList_GetItem (cont, k-1));
    text_size = l;

    // Free the three lists
    Py_XDECREF (npc);
    Py_XDECREF (player);
    Py_XDECREF (cont);
}

// execute embedded functions and replace shortcuts
// yeah, the c string library hurts, but at least it's fast ;)
char* dialog::scan_string (const char *s)
{
    u_int32 begin, end, len, done;
    PyObject *result;
    char *start, *mid, *string = NULL;
    char *tmp, *newstr = strdup (s);
    player *the_player = (player*) objects::get("the_player");

    // replace $... shortcuts
    while (1)
    {
        // check wether the string contains shortcut code at all
        start = strchr (newstr, '$');
        if (start == NULL) break;

        done = 0;

        // replace "$name"
        if (strncmp (start, "$name", 5) == 0)
        {
            begin = strlen (newstr) - strlen (start);
            tmp = new char[strlen (newstr) - 5 + strlen (the_player->name)];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            strcat (tmp, the_player->name);
            strcat (tmp, start+5);
            delete newstr;
            newstr = tmp;
            done = 1;
        }

        // replace "$fm"
        if (strncmp (start, "$fm", 3) == 0)
        {
            // extract the "$fm{.../...} part
            end = strcspn (start, "}");
            string = new char[end];
            string[end-1] = 0;        
            strncpy (string, start+3, end);

            if (the_player->get("gender") == 0) mid = get_substr (string, "{", "/");
            else mid = get_substr (string, "/", "}");

            begin = strlen(newstr) - strlen(start);
            tmp = new char[strlen(newstr) - end + strlen (mid)];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            strcat (tmp, mid);
            strcat (tmp, start+end+1);
            
            delete string;
            delete newstr;
            newstr = tmp;

            done = 1;
        }

        // Error!
        if (!done)
        {
            begin = strlen (newstr) - strlen (start);
            tmp = new char[begin];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            delete newstr;
            newstr = tmp;
            cout << "\n*** Error, unknown macro " << start << flush;
        }
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
        result = PyObject_CallMethod (instance, string, NULL);

        mid = NULL;

        if (result)
            if (PyString_Check (result))
                mid = PyString_AS_STRING (result);    
        
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
        delete newstr;
        newstr = tmp;

        // Cleanup
        Py_XDECREF (result);
        delete string;
    }

    return newstr;
}

char *dialog::get_substr (const char* string, char* begin, char* end)
{
    u_int32 b, e;
    b = strcspn (string, begin) + 1;
    e = strcspn (string, end);

    char *result = new char[e-b];
    strncpy (result, string+b, e);
    result[e-b] = 0;

    return result;
}

// Init the dialogue engine
dialog_engine::dialog_engine (mapcharacter *c)
{
    npc *mynpc = (npc *) c->data;

    // Init the low level dialogue stuff
    dlg = new dialog;

    // Create window
    font = new win_font (game::theme);
    border = new win_border (game::theme);
    back = new win_background (game::theme);
    wnd = new win_container (40, 20, 240, 160);

    face = wnd->add_image (5, 5, c->portrait);
    name = wnd->add_label (80, 5, 155, 10, font);
    name->set_text (mynpc->name);

    prev = wnd->add_label (80, 20, 155, 60, font);
    // sel = wnd->add_select (5, 80, 230, 75, font);

    wnd->set_border (border);
    wnd->set_background (back);
    wnd->show_all ();
    wnd->draw ();

    // Load dialogue
	if (!dlg->init (mynpc->talk (), strrchr (mynpc->talk (), '/')+1))
	{
        cout << "\n*** Error loading dialogue script " << strrchr (mynpc->talk (), '/')+1 << flush;
        answer = -1;	
	}
	else answer = 0;
}

void dialog_engine::update_keyboard ()
{
/*
	static u_int16 lastkeypressed=0;
	
	if(input::is_pushed(lastkeypressed)) return;
	lastkeypressed=0;

	if (input::is_pushed (97)) 
	{dlg->answer = 0; lastkeypressed=97;}
	if (input::is_pushed (98)) 
	{dlg->answer = 1; lastkeypressed=98;}
	if (input::is_pushed (99)) 
	{dlg->answer = 2; lastkeypressed=99;}
	if (input::is_pushed (100)) 
	{dlg->answer = 3; lastkeypressed=100;}
*/
}

void dialog_engine::update ()
{
    // wnd->update ();
    wnd->draw ();
/*
    if (dlg->answer == 0xFFFF) return;

    run (win);
    dlg->answer = 0xFFFF;
*/
}

void dialog_engine::run ()
{
    // Error occured:
    if (answer < 0) return;
}

void dialog_engine::insert_plugin ()
{
}
