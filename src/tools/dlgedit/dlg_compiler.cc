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
 * @file dlg_compiler.cc
 *
 * @author Kai Sterker
 * @brief The dialogue compiler. 
 */

#include "dlg_compiler.h"
#include "dlg_types.h"

DlgCompiler::DlgCompiler (DlgModule *module)
{
    dialogue = module;
}

void DlgCompiler::run ()
{
    // nothing to do if there is no dialogue
    if (dialogue == NULL) return;
    
    // try to open the file
    string fname = module->name ();

    // remove the serial number from the name
    unsigned int pos = file.rfind ("-");
    if (pos != file.npos) file.erase (pos);

    // try to open the file
    if (!file.open (fname)) return;
    
    // write the script header
    writeHeader (g_basename (fname.c_str ()));
    
    // write the dialogue text
    writeText ();
    
    // write the start of the dialoge
    writeStart ();
}

void DlgCompiler::writeHeader (string &theClass)
{
    // imports
    file << "import dialogue\n\n"
            
    // stuff needed for i18n
         << "# -- pygettext support\n"
         << "def _(message): return message\n\n"
    
    // the classname
         << "class " << theClass << " (dialogue.base):\n";
}

// write array with the dialogue text and initialize some stuff
void DlgCompiler::writeText ()
{
    vector<DlgNode*> nodes = module->getNodes ();
    vector<DlgNode*>::iterator i = nodes.begin ();
    unsigned int j = 0;
    DlgCircleEntry *entry;
    
    // the array with the dialogue text
    file << "\ttext = [\\\n";
    
    for (i; i != nodes.end (); i++)
    {
        // nothing to be done for Arrows
        if ((*i)->type () == LINK) continue;
    
        // get the entry of the current circle
        entry = ((DlgCircle *) *i)->entry ();
        
        // see whether this is a start-node
        if ((*i)->prev (FIRST) == NULL)
            addStart (*i);
                
        // build up condition vector
        if (entry->condition () != "")
            addCondition (entry->condition ());
    
        // build up code vector
        if (entry->code () != "")
            addCode (entry->code ());
        
        // set index of this node for later use
        (*i)->setIndex (j++);
        
        // write text of the node
        file << "\t\t_(\"" << entry->text () << "\"),\\\n";
    }
    
    // close text array
    file << "\t\tNone]\n\n";
}

// write the start of the dialogue
void DlgCompiler::writeStart ()
{
    // begin dialogue array
    file << "\t# -- (code, character, stop, (text, logical op, condition, ...))
         << "\tdlg = [\\\n"
         << "\t\t(None, None, 0, (";
    
    // write the "followers" (in this case the start nodes)
        
}

// add node to the dialogue's start nodes
void DlgCompiler::addStart (DlgNode *node)
{
    vector<DlgNode*>::iterator i = start.begin ();

    // search the proper place for insertion
    if (!start.empty ()) 
        while (i != start.end () && *(*i) < *node) i++;
    
    // insert
    start.insert (i, node);
}

// add a condition to the list of conditions
void DlgCompiler::addCondition (string cnd)
{
    condition.push_back (cnd);
}

// add arbitrary code to the list of code
void DlgCompiler::addCode (string cde)
{
    code.push_back (cde);
}

