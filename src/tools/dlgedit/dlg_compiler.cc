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

#include <iterator>
#include "dlg_compiler.h"
#include "dlg_types.h"

DlgCompiler::DlgCompiler (DlgModule *module)
{
    dialogue = module;
    
    int length = module->getNodes ().size () + 1;
    
    // create our lookup tables    
    codeTable = new int[length];
    conditionTable = new int[length];
    operationTable = new int[length];
    
    // initialize them with something sensible
    memset (codeTable, 255, length * sizeof (int));
    memset (conditionTable, 255, length * sizeof (int));
    memset (operationTable, 0, length * sizeof (int));
}

// dtor
DlgCompiler::~DlgCompiler ()
{
    delete[] codeTable;
    delete[] conditionTable;
    delete[] operationTable;
}

// compile the dialogue into Python script
void DlgCompiler::run ()
{
    // try to open the file
    string fname = dialogue->name ();

    // remove the serial number from the name
    unsigned int pos = fname.rfind ("-");
    if (pos != fname.npos) fname.erase (pos);

    // try to open the file
    file.open ((fname + ".py").c_str ());
    if (file.eof ()) return;
    
    // write the script header
    writeHeader (g_basename (fname.c_str ()));
    
    // write the dialogue text
    writeText ();
    
    // write the conditions
    writeConditions ();
    
    // write the code
    writeCode ();
    
    // write the start of the dialoge
    writeStart ();
    
    // write the rest of the dialogue
    writeDialogue ();
}

void DlgCompiler::writeHeader (const string &theClass)
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
    vector<DlgNode*> nodes = dialogue->getNodes ();
    vector<DlgNode*>::iterator i;
    DlgCircleEntry *entry;
    unsigned int j = 0;
    
    // the array with the dialogue text
    file << "\ttext = [None";
    
    for (i = nodes.begin (); i != nodes.end (); i++)
    {
        // nothing to be done for Arrows
        if ((*i)->type () == LINK) continue;
    
        // get the entry of the current circle
        entry = ((DlgCircle *) *i)->entry ();
        
        // see whether this is a start-node
        if ((*i)->prev (FIRST) == NULL)
            addStart (*i);
                
        // set index of this node for later use
        (*i)->setIndex (++j);

        // build up condition vector
        if (entry->condition () != "")
            if (!addCondition (entry->condition (), j))
                cout << "\"" << entry->text () << "\"\n" << flush;
    
        // build up code vector
        if (entry->code () != "")
            addCode (entry->code (), j);
        
        // write text of the node
        file << ",\\\n\t\t_(\"" << entry->text () << "\")";
    }
    
    // close text array
    file << "]\n\n";
}

// write conditions of the dialogue
void DlgCompiler::writeConditions ()
{
    // nothing to do if there are no conditions
    if (conditions.empty ()) return;
    
    file << "\tcond = [\\";

    // write the individual conditions    
    for (vector<string>::iterator i = conditions.begin (); i != conditions.end (); i++)
    {
        if (i != conditions.begin ()) file << ",\\";
        file << "\n\t\t\"" << (*i) << "\"";
    }
    
    // close array
    file << "]\n\n";
} 

// write code of the dialogue
void DlgCompiler::writeCode ()
{
    // nothing to do if there are no conditions
    if (code.empty ()) return;
    
    file << "\tcode = [\\";

    // write the individual conditions    
    for (vector<string>::iterator i = code.begin (); i != code.end (); i++)
    {
        if (i != conditions.begin ()) file << ",\\";
        file << "\n\t\t\"" << (*i) << "\"";
    }
    
    // close array
    file << "]\n\n";
}
 
// write the start of the dialogue
void DlgCompiler::writeStart ()
{
    // begin dialogue array
    file << "\t# -- (speaker, code, stop, (text, operation, condition, ...))"
         << "\n\tdlg = [\\\n"
         << "\t\t(None, -1, 1, (";
    
    // write the "followers" (in this case the start nodes)
    if (!start.empty ())
    {
        vector<DlgNode*>::iterator i;

        for (i = start.begin (); i != start.end (); i++)
        {
            if (i != start.begin ()) file << ", ";
            writeFollower (*i);
        }
    }
}

// write the actual dialogue data
void DlgCompiler::writeDialogue ()
{
    vector<DlgNode*> nodes = dialogue->getNodes ();
    vector<DlgNode*>::iterator i;
    DlgCircle *circle, *child;
    DlgCircleEntry *entry;
    int index;
        
    for (i = nodes.begin (); i != nodes.end (); i++)
    {
        // nothing to be done for Arrows
        if ((*i)->type () == LINK) continue;
    
        circle = (DlgCircle *) (*i);
        
        // get the entry of the current circle
        entry = circle->entry ();
        
        file << ")),\\\n\t\t(";
        
        // write Speaker
        switch (circle->type ())
        {
            case NPC:
                file << "\"" << entry->npc () << "\"";
                break;
                
            case NARRATOR:
                file << "\"Narrator\"";
                break;
            
            default:
                file << "None";
        }
        
        // write code
        file << ", " << codeTable[circle->index ()] << ", "
                
        // write action
             << checkFollowers (circle) << ", (";
        
        // write all followers
        index = 0;
        for (child = circle->child (FIRST); child != NULL; child = circle->child (NEXT), index++)
        {
            if (index != 0) file << ", ";
            writeFollower (child);
        }
    }
    
    // end of array
    file << "))]\n\n";
} 

// write a single follower
void DlgCompiler::writeFollower (DlgNode *node)
{
    file << node->index () << ", " << operationTable[node->index ()] 
         << ", " << conditionTable[node->index ()];
}

// add node to the dialogue's start nodes
void DlgCompiler::addStart (DlgNode *node)
{
    vector<DlgNode*>::iterator i = start.begin ();

    // search the proper place for insertion
    if (!start.empty ()) 
        while (i != start.end () && *node < *(*i)) i++;
    
    // insert
    start.insert (i, node);
}

// add a condition to the list of conditions
bool DlgCompiler::addCondition (const string &cnd, int idx)
{
    string condition;
    bool retval = true;
    
    // see what kind of statement the condition is and get rid of the keyword
    if (strncmp ("if ", cnd.c_str (), 3) == 0)
    {
        condition = cnd.substr (3);
    }
    else if (strncmp ("elif ", cnd.c_str (), 5) == 0)
    {
        condition = cnd.substr (5);
        operationTable[idx] = 1;
    }
    else if (strncmp ("else:", cnd.c_str (), 5) == 0)
    {
        operationTable[idx] = 1;
        return true;        
    }
    else
    {
        // a condition that doesn't start with any of the above is wrong
        cout << "*** Compiler: Error in condition\n    \"" << cnd
             << "\" of node\n    ";
        return false;
    }
    
    // now get rid of the colon at the end of the condition
    if (condition[condition.size () - 1] == ':')
        condition.erase (condition.size () - 1);
    // if there is none, that's not too tragical, but report it anyway
    else
    {
        cout << "*** Compiler: Colon missing in condition\n    \"" << cnd
             << "\" of node\n    ";
        retval = false;
    }
    
    // now the condition is ready for addition to the condition vector
    for (vector<string>::iterator i = conditions.begin (); i != conditions.end (); i++)
        if (strcmp ((*i).c_str (), condition.c_str ()) == 0)
        {
            conditionTable[idx] = distance (conditions.begin (), i);
            return retval;
        }
    
    // the condition isn't in the table so far, so add it
    conditionTable[idx] = conditions.size ();
    conditions.push_back (condition);
    
    return retval;
}

// add arbitrary code to the list of code
void DlgCompiler::addCode (const string &cde, int idx)
{
    // see if code like this already exists
    for (vector<string>::iterator i = code.begin (); i != code.end (); i++)
        if (strcmp ((*i).c_str (), cde.c_str ()) == 0)
        {
            codeTable[idx] = distance (code.begin (), i);
            return;
        }
    
    // the code isn't in the table so far, so add it
    codeTable[idx] = code.size ();
    code.push_back (cde);
}

// check whether PLAYER or NPC/NARRATOR nodes follow the given node
int DlgCompiler::checkFollowers (DlgCircle *circle)
{
    DlgNode *child = circle->child (FIRST);
    if (child == NULL) return 1;
    
    node_type type = child->type ();
    
    // make sure that the followers are consistent
    for (; child != NULL; child = circle->child (NEXT))
    {
        if (type == child->type ()) continue;
        
        // that's an error
        if ((type == PLAYER && child->type () != PLAYER) ||
            (type != PLAYER && child->type () == PLAYER))
        {
            cout << "*** Compiler: Followers must not mix PLAYER and NPC" 
                 << "nodes in node\n    \"" << circle->entry ()->text () 
                 << "\"\n" << flush;
            break;
        }
    }
    
    if (type == PLAYER) return 0;
    else return 1;
}
