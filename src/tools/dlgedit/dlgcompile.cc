/*
   $Id$
   
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/ 

#include <ctype.h>
#include <string.h>
#include <algorithm>

#include "../../types.h"
#include "../../data.h"
#include "dlgnode.h"
#include "dlgcompile.h"

string dlg_compiler::operators[NUM_OPS] = { "and", "or", "==", "!=", "<", "<=", ">", 
    ">=", "+", "-", "*", "/", "%", "&", "|", "^", "."};

dlg_compiler::dlg_compiler (vector<DlgNode*> &d, string f, string c, string i, u_int8 dbg)
{
    dlg = d;
    debug = dbg;
    filename = f;
    cust_func = c;
    cust_init = i;

    text_lookup = new u_int32[dlg.size ()];
    jump_lookup = new s_int32[dlg.size ()];
}

dlg_compiler::~dlg_compiler ()
{
    if (text_lookup) delete text_lookup;
    if (jump_lookup) delete jump_lookup;
}

// Compile the Dialogue
void dlg_compiler::run ()
{
    script.open ((filename + ".py").c_str ());
    
    // write the strings used in the dialogue
    write_strings ();

    // write the dialogue functions
    write_dialogue ();

    // this creates the script's "entry function"
    write_entry_func ();

    // write start
    write_start ();

    // write all the answers
    while (!todo_nodes.empty ())
    {
        get_cur_nodes ();
        write_answer ();
    }
    
    script.close ();
}

// Write the strings used in the dialogue
void dlg_compiler::write_strings ()
{
    vector<DlgNode*>::iterator i;
    u_int32 j = 0;

    if (debug) script << "from types import *\n\n";

    // write the class name
    script << "class " << strrchr (filename.c_str (), '/') + 1 << ":\n"
           << "    loop = []\n"
           << "    strings = [";

    // write all strings and build lookup table to get index when given
    // the number of the node
    for (i = dlg.begin (); i != dlg.end (); i++)
        if ((*i)->type != LINK)
        {
            script << "\""<< ((Circle *)(*i))->text.c_str () << "\", ";
            text_lookup[(*i)->number] = j++;
        }

    // with a last, empty string we don't have to care about the final comma
    script << "\"\"]\n\n";

    // these are dummies for the dialog editor; they're overwritten by
    // the game's dialogue engine
    script << "    def set_name (self, new_name):\n"
           << "        pass\n\n"
           << "    def set_npc (self, new_npc):\n"
           << "        pass\n\n"
           << "    def set_portrait (self, new_portrait):\n"
           << "        pass\n";

    // This holds debug info
    if (debug)
    {
        script << "\n    debug_info = {}\n\n"
               << "    def set_var (self, name, val):\n"
               << "        self.__dict__[name] = val\n"
               << "        self.debug_info[name] = val\n";
    }
    
    return;
}

// Write the dialogue functions
void dlg_compiler::write_dialogue ()
{
    vector<DlgNode*>::iterator i;
    u_int32 j = 0;

    // write the dialogue functions
    script << "\n    def __init__(self):";
    script << "\n        self.dialogue = [self.start, ";

    // write answer-function-array and build lookup table to get index when given
    // the number of the node
    for (i = dlg.begin (); i != dlg.end (); i++)
        if ((*i)->type != LINK && npc_follows ((Circle*)*i))
        {
            script << "self.answer" << (*i)->number << ", ";
            jump_lookup[(*i)->number] = ++j;
        }
        else jump_lookup[(*i)->number] = -1;

    // with a last, empty string we don't have to care about the final comma
    script << "None]";

    // write user-supplied __init__ code if any
    if (cust_init != "")
    {
        space = "        ";
        write_custom_code (cust_init);
        script << "\n";
    }
    
    // write user-supplied methods if any
    if (cust_func != "") 
    {
        space = "    ";
        write_custom_code (cust_func);
        script << "\n";
    }
    
    return;
}

void dlg_compiler::write_entry_func ()
{
    // overwrite __getattr__, so that member variables need not be
    // defined before using them
    script << "\n    def __getattr__ (self, name):";
    if (debug) script << "\n        print \"*** Warning: \\\"\" + name + \"\\\" not defined!\"";
    script << "\n        return 0\n";

    // For debugging, also overwrite __setattr__, so we always know which variables
    // are defined
    if (debug)
    {
        script << "\n    def __setattr__ (self, name, value):"
               << "\n        self.__dict__[name] = value"
               << "\n        if type (value) is IntType:"
               << "\n            self.debug_info[name] = value\n"
               << "\n    __debug__ = 1\n";
    }
    
    // Write the function to start/continue the dialogue
    script << "\n    def run (self, answer):"
           << "\n        self.npc = []"
           << "\n        self.player = []"
           << "\n        self.cont = []"
           << "\n        self.dialogue[answer]()\n";
}

// write additional user defined functions (if any)
void dlg_compiler::write_custom_code (string code)
{
    u_int32 i = 0, j;

    code += '\n';
        
    while ((j = code.find ('\n', i)) < code.size ())
    {
        script << "\n" << space << inflate (code.substr (i,j-i), RGET);
        i = ++j;
    }

    code.erase (code.end()-1);

    script << "\n";
}

// Inflate short code like "a_quest.attribute = 4" to the proper
// quests.get ("a_quest").set ("attribute", 4):
//
// First we transform the code into a binary tree, where each inner node
// is an operator like 'and', 'or', '==', ... and each leaf either a
// variable or constant.
//
// Then we traverse the tree in order to reassemble the code. On
// that occasion we replace var with 'get ("var")' or set '("var",' 
// accordingly.
//
// Oh, and instead of building an actual tree, we simply use recusion
// - that's much more fun! :)
string dlg_compiler::inflate (string code, int mode)
{
    u_int32 i, pos;
    string left;

    // '=' (but not '!=', '<=', '>=' or '==')
    if ((pos = code.find ("=")) != code.npos && code[pos-1] != '!' &&
        code[pos-1] != '<' && code[pos-1] != '>' && code[pos+1] != '=')
    {
        left = code.substr (0, pos);

        // don't break assignments to local variables
        if (left.find (".") == left.npos || left.find ("self.") != left.npos) 
            return left + string ("=") + inflate (code.substr (pos + 1), RGET); 
        else
            return inflate (left, SET) + string (", ") + 
                inflate (code.substr (pos + 1), RGET) + ")"; 
    }
    
    // everything 'cept '='
    for (i = 0; i < NUM_OPS; i++)
        if ((pos = code.find (operators[i])) != code.npos)
        {
            left = inflate (code.substr (0, pos), LGET);

            // don't break reads from local variables
            if (operators[i] == "." && left.find ("self") != left.npos)
                return left + operators[i] + code.substr (pos + operators[i].length());
            else
                return left + operators[i] + inflate (code.substr (pos +
                    operators[i].length()), mode == SET ? SET : RGET);
        }
        
    // if we reach this point, we're in a leaf

    // if the code already contains 'set' or 'get' there'll be a pair 
    // of quotes or brackets and we've got nothing to do
    if ((code.find ('"') != code.npos || code.find ('\'') != code.npos) ||
        (code.find ('(') != code.npos && code.find (')') != code.npos))
        return code;

    string variable, prefix(""), suffix("");

    switch (mode)
    {
        // if we're in the left child, we gotta watch out for 'self',
        // 'the_player' and 'the_npc'. Further, we might have an 'if',
        // 'elif' or 'else' in  front of our variable
        case LGET:
        {
            // handle trailing whitespace
            pos = code.length()-1;
            while (code[pos] == ' ') pos--;
            
            if (pos != code.length()-1)
            {
                suffix = code.substr (pos+1);
                code.erase (pos+1);
            }
            
            pos = 0;

            while (pos != code.npos)
            {
                variable = code.substr (pos == 0 ? 0 : pos + 1);
                prefix = code.substr (0, pos == 0 ? 0 : pos + 1);
#ifdef _DEBUG_
                cout << "*** c: " << code << " p: " << prefix << " v: " << variable << endl;
#endif
                if (variable != "the_player" && variable != "the_npc" && 
                    variable != "self")
                {
                    // check whether we access the quest- or character array
                    if (data::quests.get (variable.c_str()) != NULL)
                        return prefix + string ("quests.get (\"") + variable + 
                            string ("\")") + suffix;

                    if (data::characters.get (variable.c_str()) != NULL)
                        return prefix + string ("characters.get (\"") + variable +
                            string ("\")") + suffix;                         
                }

                pos = code.find (' ', pos + 1);
            }

            break;
        }

        // in a right child, we might have a ':' or ')' following the variable
        case SET:
        case RGET:
        {
            pos = 0;
            
            // handle leading whitespace
            while (code[pos] == ' ') pos++;

            if (pos)
            {
                prefix = code.substr (0, pos);
                code.erase (0, pos);
            }

            // extract the variable
            pos = code.find_last_not_of (":) ") + 1;

            suffix = code.substr (pos);
            variable = code.substr (0, pos);

            // don't mess with constants
            if ((variable[0] == '-' && isdigit (variable[variable.length()-1])) ||
                isdigit (variable[0]))
                return prefix + code;

            // ignore 'else'
            if (variable == "else") return prefix + code;
#ifdef _DEBUG_
            cout << "*** c: " << code << " v: " << variable << " s: " << suffix << endl;
#endif
            if (mode == SET)
                return prefix + string ("set (\"") + variable + "\"" + suffix;
            else
                return prefix + string ("get (\"") + variable + "\")" + suffix;
        }
    }

    // actually, we should never reach this
    return code;
}

// Write a NPC part 
void dlg_compiler::write_npc (Circle *circle)
{
    u_int32 changed = 0;

    // set NPC if changed
    if (character_changed (circle))
    {
        changed = 1;
        
        script << "\n" << space << "self.set_npc (";
        if (circle->character == "") script << "the_npc.get_name())";
        else script << "\"" << circle->character << "\")";
    }

    // set the text color
    if (circle->type == NARRATOR)
    {
        script << "\n" << space << "self.color = 0";
    }
    else
    {
        script << "\n" << space << "self.color = ";
        if (circle->character == "") script << "the_npc.get_color()";
        else script << "characters[\"" << circle->character << "\"].get_color()";
    }

    if (debug) script << "\n\n" << space << "# " << circle->text.c_str (); 

    // write circle's text and "jump target"
    script << "\n" << space << "self.npc.append (" << text_lookup[circle->number] << ")"
           << "\n" << space << "self.cont.append (" << jump_lookup[circle->number] << ")";

    // write circle's additional code (if any)
    if (circle->variables != "")
        write_custom_code (circle->variables);

    // allow loops
    if (circle->actions[0] == '1')
        script << "\n" << space << "self.loop.append (" << circle->number << ")";
}

// Write a player part
void dlg_compiler::write_player (Circle *circle)
{
    // write circle's condition (if any)
    if (circle->conditions != "") 
    {
        write_custom_code (circle->conditions);
        space += "    ";
    }

    if (debug) script << "\n\n" << space << "# " << circle->text.c_str (); 

    // write circle's text and "jump target"
    script << "\n" << space << "self.player.append (" << text_lookup[circle->number] << ")"
           << "\n" << space << "self.cont.append (" << jump_lookup[circle->number] << ")";

    // allow loops
    if (circle->actions[0] == '1')
        script << "\n" << space << "self.loop.append (" << circle->number << ")";

    if (circle->conditions != "") space.erase (space.end()-4,space.end()); 
}

// Write the first bit of the dialogue
void dlg_compiler::write_start ()
{
    vector<DlgNode*>::iterator i;

    // find the start of the dialogue (all nodes without parent)
    for (i = dlg.begin(); i != dlg.end (); i++)
        if ((*i)->type != LINK)
            if ((*i)->prev.empty ())
                cur_nodes.push_back (*i);

    sort (cur_nodes);
    
    script << "\n    def start (self):";
    write_answer ();

    return;
}

// all the NPC->nodes to add to the function are stored in cur_nodes
// at that moment
void dlg_compiler::write_answer ()
{
    vector<DlgNode*>::iterator i, j;
    space = "        ";

    // write npc node & followers (if any)
    for (i = cur_nodes.begin (); i != cur_nodes.end (); i++)
    {
        // If we find a PLAYER node here, there is something wrong
        if ((*i)->type == PLAYER)
        {
            int type = PLAYER;

            // Find out which of the two possible errors (below) we have here
            for (j = cur_nodes.begin (); j != cur_nodes.end (); j++)
                if ((*j)->type != PLAYER)
                {
                    type = (*j)->type;
                    break;
                }

            cout << "\n*** Compile error: NPC node expected, but found PLAYER node\n"
                 << "\"" << ((Circle *) (*i))->text << "\"\n";

            // A node's siblings have to be of the same kind
            if (type != PLAYER)
                 cout << "A node can't have different types of siblings." << flush;

            // Two Player nodes following each other isn't allowed
            else 
                cout << "A PLAYER node may not follow another PLAYER node." << flush;
        }
        
        // write circle's condition (if any)
        if (((Circle*)(*i))->conditions != "") 
        {
            write_custom_code (((Circle*)(*i))->conditions);
            space = "            ";
        }   

        // Write NPC stuff
        write_npc ((Circle*)*i);

        // write following player nodes (if any)
        if (player_follows (*i))
            write_player_answer (*i);
            
        // if npc node(s) follow instead, add node for further actions
        if (npc_follows (*i))
            if (find (todo_nodes.begin(), todo_nodes.end (), *i) == todo_nodes.end () &&
                find (done_nodes.begin(), done_nodes.end (), *i) == done_nodes.end ())
                todo_nodes.push_back (*i);

        // add a delimiter to distinguish between player text's following
        // different NPC answers
        script << "\n" << space << "self.player.append (-1)";
    }

    if (cur_nodes.empty ())
        script << "\n" << space << "pass";
    
    script << "\n";

    return;
}

// write the player's text following a npc node
void dlg_compiler::write_player_answer (DlgNode *npc)
{
    vector<DlgNode*>::iterator i;
    vector<DlgNode*> answers;

    // copy direct and indirect links to a single array
    for (i = npc->next.begin (); i != npc->next.end (); i++)
        answers.push_back ((*i)->next[0]);

    for (i = npc->link.begin (); i != npc->link.end (); i++)
        answers.push_back ((*i)->next[0]);

    if (answers.size () > 1) sort (answers);

    // write the code
    for (i = answers.begin (); i != answers.end (); i++)
    {
        write_player ((Circle*) *i);
        
        // if node isn't already handled or queued for handling,
        // add it to todo_nodes to have it handled later
        if (find (todo_nodes.begin(), todo_nodes.end (), *i) == todo_nodes.end () &&
            find (done_nodes.begin(), done_nodes.end (), *i) == done_nodes.end ())
            todo_nodes.push_back (*i);
    }

    return;
}

// Get the nodes that make the next answer
void dlg_compiler::get_cur_nodes ()
{
    vector<DlgNode*>::iterator i;
    Circle *circle = (Circle *) todo_nodes.back ();

    cur_nodes.clear ();
    todo_nodes.pop_back ();
    done_nodes.push_back (circle);

    // For all following direct links (arrows) ...
    for (i = circle->next.begin (); i != circle->next.end (); i++)
        // Add the NPC-node to the nodes to compile next
        cur_nodes.push_back ((*i)->next[0]);
        
    // Now the same for the indirect followers
    for (i = circle->link.begin (); i != circle->link.end (); i++)
        cur_nodes.push_back ((*i)->next[0]);

    if (cur_nodes.size () > 1) sort (cur_nodes);
    
    script << "\n    def answer" << circle->number << " (self):";

    // write player's additional code
    if (circle->type == PLAYER && circle->variables != "")
    {
        space = "        ";
        write_custom_code (circle->variables);
    }
    
    return;
}

// Get if - [elif ...] - else statements in the right order 
void dlg_compiler::sort (vector<DlgNode*> &v)
{
    u_int32 i, if_pos = 0;
    DlgNode *node;

    for (i = 0; i < v.size (); i++)
    {
        node = v[i];

        // move everything without condition to the front
        if (((Circle *) node)->conditions == "")
        {
            if (i != if_pos)
            {
                v.erase (remove (v.begin (), v.end (), node), v.end ());
                v.insert (v.begin (), node);
            }

            if_pos++;
        }
        else
        {
            // move "else" to the back
            if (v.size ()-1 != i && !strncmp (((Circle *) node)->conditions.c_str (), "else", 4))
            {
                v.erase (remove (v.begin (), v.end (), node), v.end ());
                v.insert (v.end (), node);
                i--;
            }

            // insert "if" after those nodes without condition 
            if (if_pos != i && !strncmp (((Circle *) node)->conditions.c_str (), "if", 2))
            {
                v.erase (remove (v.begin (), v.end (), node), v.end ());
                v.insert (v.begin () + if_pos, node);
            }
        }
    }
}

// Check wether player text follows after that NPC node
u_int8 dlg_compiler::player_follows (DlgNode *circle)
{
    // Check immediate followers
    if (circle->next.size () > 0)
        if (circle->next[0]->next[0]->type == PLAYER)
            return 1;

    // Check linked followers
    if (circle->link.size () > 0)
        if (circle->link[0]->next[0]->type == PLAYER)
            return 1;

    return 0;
}

u_int8 dlg_compiler::npc_follows (DlgNode *circle)
{
    // Check immediate followers
    if (circle->next.size () > 0)
        if (circle->next[0]->next[0]->type != PLAYER)
            return 1;

    // Check linked followers
    if (circle->link.size () > 0)
        if (circle->link[0]->next[0]->type != PLAYER)
            return 1;

    return 0;
}

u_int8 dlg_compiler::character_changed (Circle *circle)
{
    if (circle->prev.empty ()) return 0;

    vector<Circle*> prevs;
    vector<Circle*>::iterator i;

    // get all direct precedessors
    get_prev_npc_nodes (circle, prevs);

    // get all indirect precedessors
    get_prev_npc_links (circle, prevs);

    for (i = prevs.begin (); i != prevs.end (); i++)
        if ((*i)->character != circle->character)
            return 1;

    return 0;
}

u_int8 dlg_compiler::narrator_before (Circle *circle)
{
    if (circle->prev.empty ()) return 0;

    vector<Circle*> prevs;
    vector<Circle*>::iterator i;

    // get all direct precedessors
    get_prev_npc_nodes (circle, prevs);

    // get all indirect precedessors
    get_prev_npc_links (circle, prevs);

    for (i = prevs.begin (); i != prevs.end (); i++)
        if ((*i)->type == NARRATOR)
            return 1;

    return 0;
}

void dlg_compiler::get_prev_npc_nodes (Circle* circle, vector<Circle*> &prevs)
{
    DlgNode *node;
    vector<DlgNode*>::iterator i;

    // for all the circle's direct precedessor's
    for (i = circle->prev.begin (); i != circle->prev.end (); i++)
    {
        node = (*i)->prev[0];

        // ... add all NPC nodes to prevs
        if (node->type == PLAYER)
        {
            // We have to check both immediate and linked nodes
            get_prev_npc_nodes ((Circle *) node, prevs);
            get_prev_npc_links ((Circle *) node, prevs);
        }
        else prevs.push_back ((Circle *) node);
    }
}

void dlg_compiler::get_prev_npc_links (Circle* circle, vector<Circle*> &prevs)
{
    vector<DlgNode*>::iterator i, j;

    for (i = circle->prev.begin (); i != circle->prev.end (); i++)
        for (j = (*i)->link.begin (); j != (*i)->link.end (); j++)
        {
            if ((*j)->type == PLAYER) 
            {
                get_prev_npc_nodes ((Circle *) (*j), prevs);
                get_prev_npc_links ((Circle *) (*j), prevs);
            }
            else prevs.push_back ((Circle *) (*j));
        }
}
