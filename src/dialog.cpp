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
#include "game.h"
#include "py_inc.h"
#include "yarg.h"
#include "input.h"
#include "dialog.h"


// Load and instanciate the dialogue object
bool dialog::init (char *fpath, char *name)
{
    PyObject *module;
	PyObject *classobj;

	// First, test if the module has already been imported

	// Seems not, so import
	module = import_module (fpath);

	if (!module)
		return false;

    module = PyImport_ReloadModule (module);
    PyObject *globals = PyModule_GetDict (module);

	// Extract the class from the dialogue module
	classobj = PyObject_GetAttrString(module, name);

	if (!classobj)
		return false;

	Py_DECREF(module);

	// Instantiate! Will we ever need to pass args to class
	// constructor here?
	instance = PyObject_CallObject(classobj, NULL);

    // add some stuff to the dialogue's global namespace
    PyObject *characters = PyDict_GetItemString (game::globals, "characters");
    PyObject *quests = PyDict_GetItemString (game::globals, "quests");
    PyObject *the_npc = PyDict_GetItemString (game::globals, "the_npc");
    PyObject *the_player = PyDict_GetItemString (game::globals, "the_player");
    
    PyDict_SetItemString (globals, "characters", characters);
    PyDict_SetItemString (globals, "quests", quests);
    PyDict_SetItemString (globals, "the_npc", the_npc);
    PyDict_SetItemString (globals, "the_player", the_player);

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

PyObject* dialog::get_instance ()
{
    return instance;
}

// Gets the index of either the player or npc array
void dialog::run (u_int32 index)
{
    u_int32 nsz, psz, i, j = 0, k = 0, l = 1;
    yarg randgen;
    s_int32 s;
    
    // Is it sufficient to get those objects only once???
    PyObject *npc, *player, *cont;

    // (Re)Init dialog::text
    if (text)
    {
        for (i = 0; i < text_size; i++) delete text[i];
        delete text;

        text_size = 0;
        text = NULL;
    }

    // End of dialogue:
    if (answers[index] == -1) return;
    
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

    nsz = PyList_Size (npc);
    psz = PyList_Size (player);

    if (nsz != 0)
    {
        text = new char*[nsz+psz];
    
        // 3. Randomly chose between possible NPC replies
        randgen.init (" ", 0, nsz-1);
    }
    // End of dialogue
    else return;

    randgen.randomize ();
    i = randgen.get (5);

    // The first value of text is the NPC Part
    s = PyInt_AsLong (PyList_GetItem (npc, i));

    // scan the string for { python code }
    text[0] = scan_string (strings[s]);
    answers.push_back (-1);

    // 4. Mark the NPC text as used unless it's allowed to loop
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
    u_int32 begin, end, len;
    PyObject *result;
    char *start, *mid, *string = NULL;
    char *tmp, *newstr = strdup (s);
    player *the_player = (player*) game::characters.get("the_player");

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
            tmp = new char[strlen (newstr) - 5 + strlen (the_player->name)];
            strncpy (tmp, newstr, begin);
            tmp[begin] = 0;
            strcat (tmp, the_player->name);
            strcat (tmp, start+5);
            delete newstr;
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

            if (the_player->get("gender") == FEMALE) mid = get_substr (string, "{", "/");
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
dialog_engine::dialog_engine (const char *npc_name)
{
    // save dialogue engine
    engine = game::engine;
    game::engine = this;
    portrait = NULL;
    name = NULL;
    face = NULL;
    
    npc *mynpc = (npc *) game::characters.get (npc_name);
    char *dlg_file = mynpc->get_dialogue ();
    
    // Init the low level dialogue stuff
    dlg = new dialog;

    // Create window
    font = new win_font (game::theme);
    border = new win_border (game::theme,WIN_BORDER_MINI_SIZE);
    cursor = new win_cursor (game::theme);
    back = new win_background (game::theme);
    wnd = new win_container (40, 20, 240, 160);

    // set the NPC's portrait (later:  get the portrait to use from the npc data)
    set_portrait ("gfxtree/portraits/lyanna.pnm");
    // ... and name
    set_name (mynpc->name);
        
    txt = wnd->add_container (80, 5, 155, 150);
    sel = new win_select (txt);
    sel->set_cursor (cursor);
    sel->set_border (border);
    
    wnd->set_border (border);
    wnd->set_background (back);
    wnd->show_all ();
    wnd->draw ();

    // Load dialogue
	if (!dlg->init (dlg_file, strrchr (dlg_file, '/')+1))
	{
        cout << "\n*** Error loading dialogue script " << strrchr (dlg_file, '/')+1 << flush;
        answer = -1;	
	}
	else
	{
        // Make the set_portrait/name/color functions available to the dialogue script
        instance = pass_instance (this, "dialog_engine");

        PyObject *setname = PyObject_GetAttrString (instance, "set_name");
        PyObject *setportrait = PyObject_GetAttrString (instance, "set_portrait");
        PyObject *setcolor = PyObject_GetAttrString (instance, "set_color");
        PyObject *setnpc = PyObject_GetAttrString (instance, "set_npc");

        PyObject *dlg_instance = dlg->get_instance ();

        PyObject_SetAttrString (dlg_instance, "set_name", setname);
        PyObject_SetAttrString (dlg_instance, "set_portrait", setportrait);
        PyObject_SetAttrString (dlg_instance, "set_color", setcolor);
        PyObject_SetAttrString (dlg_instance, "set_npc", setnpc);
    
	   answer = 0;
    }
    delete dlg_file;
}

dialog_engine::~dialog_engine ()
{
    // refresh screen
    screen::drawbox(0,0,320,200,0);

    // restore engine
    game::engine = engine;

    delete wnd;
    delete border;
    delete cursor;
    delete portrait;
    delete font;
    delete back;

    Py_XDECREF (instance);
}

void dialog_engine::run ()
{
    u_int32 i, h = 10;
    win_label *l;
    
    // Possibly error
    if (answer < 0)
    {
        delete this;
        return;
    }

    // Continue dialogue with selected answer
    dlg->run (answer);

    // End of dialogue
    if (!dlg->text)
    {
        delete this;
        return;
    }

    // Add NPC text and all player reactions to container
    for (i = 0; i < dlg->text_size; i++)
    {
        l = txt->add_label (10, h, 120, h+font->height, font);
        l->set_auto_height (true);
        l->set_text (dlg->text[i]);
        sel->add (l, WIN_SELECT_MODE_BORDER);  
        cur_answers.push_back (l);

        h += l->height+10;
    }

    // Either select the single NPC speech ...
    if (dlg->text_size == 1) sel->set_default_obj (cur_answers.front ());
    // ... or the player's first answer
    else sel->set_default_obj (cur_answers[1]);
    
    txt->show_all ();
    txt->draw ();
}

void dialog_engine::realtime_tasks ()
{
    update_keyboard ();
}

void dialog_engine::update_keyboard ()
{
    // SPACE activates the current selection
    if (input::has_been_pushed (SDLK_SPACE))
    {
        vector<win_label*>::iterator i;

        // remember choice
        answer = sel->get_pos () - 1;

        // remove all the displayed text
        for (i = cur_answers.begin (); i != cur_answers.end (); i++)
        {
            sel->remove (*i);
            txt->remove (*i);
            delete *i;
        }
        
        cur_answers.clear ();
        run ();
    }

    // Cursor keys iterate through possible answers
    if (dlg->text_size > 1)
    {
        if (input::has_been_pushed (SDLK_UP))
        {
            if (sel->get_pos () == 2) sel->set_default_obj (cur_answers.back ());
            else sel->previous ();
        }
        if (input::has_been_pushed (SDLK_DOWN))
        {
            if (sel->get_pos () == cur_answers.size ()) sel->set_default_obj (cur_answers[1]);
            else sel->next ();    
        }
    }
}

void dialog_engine::gametime_tasks ()
{
    engine->gametime_tasks ();
    wnd->draw ();
}

void dialog_engine::insert_plugin ()
{
}

// Set / change the NPC-portrait
void dialog_engine::set_portrait (char *new_portrait)
{
    if (face)
    {
        wnd->remove (face);
        delete face;
    }
    if (portrait) delete portrait;
    
    portrait = new image (64, 64);
    portrait->load_pnm (new_portrait);
    portrait->set_mask (true);

    face = wnd->add_image (10, 10, portrait);
    face->show ();
}

// Set / change the NPC-name
void dialog_engine::set_name (char *new_name)
{
    if (name)
    {
        wnd->remove (name);
        delete name;
    }
    
    name = wnd->add_label (10, 75, 64, 10, font);
    name->set_auto_height (true);
    name->set_text (new_name);
    name->show ();
}

// Set / change the NPC-text color 
void dialog_engine::set_color (u_int32 new_color)
{
}

// Set a different NPC
void dialog_engine::set_npc (char* new_npc)
{
    npc *mynpc = (npc *) game::characters.get (new_npc);
    
    set_name (mynpc->name);
    set_portrait ("gfxtree/portraits/lyanna.pnm");
    set_color (0);
}