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
#include <ostream.h>
#include "dlg_cmdline.h"
#include "dlg_compiler.h"
#include "dlg_types.h"
#include "gui_error.h"

// Operators that may appear in Python code
std::string DlgCompiler::operators[NUM_OPS] = { "==", "!=", "<", "<=", ">", 
    ">=", "=", ".", ":", "if", "elif", "else", "pass", "return", "and", "or", 
    "not", "+", "-", "*", "/", "\"", "'", "(", ")", "[", "]", ",", "#", "%", 
    "&", "|", "^"};

// keywords that need no expansion
std::string DlgCompiler::fixed[NUM_FXD] = { "self", "quests", "the_npc", 
    "the_player", "characters"};


DlgCompiler::DlgCompiler (DlgModule *module)
{
    dialogue = module;
    errors = 0;
    
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
    if (DlgCmdline::compile == false)
    {
        // make sure the error console exists
        if (GuiError::console == NULL)
            GuiError::console = new GuiError ();
        else
            GuiError::console->clear ();
    }
    
    // try to open the file
    std::string fname = dialogue->name ();

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
    
    // write the custom code
    writeCustomCode ();
    
    // display errors if there were any
    if (DlgCmdline::compile == false && errors > 0)
        GuiError::console->display ();
}

// write the topmost part of the dialogue 
void DlgCompiler::writeHeader (const std::string &theClass)
{
    // imports
    file << "import dialogue\n";
    
    // custom imports
    if (dialogue->entry ()->imports () != "")
        file << dialogue->entry ()->imports () << "\n\n";
            
    // stuff needed for i18n
    file << "# -- pygettext support\n"
         << "def _(message): return message\n\n"
    
    // the classname
         << "class " << theClass << " (dialogue.base):\n";
}

// write array with the dialogue text and initialize some stuff
void DlgCompiler::writeText ()
{
    std::vector<DlgNode*> nodes = dialogue->getNodes ();
    std::vector<DlgNode*>::iterator i;
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
            addCondition ((DlgCircle *) *i, j);
    
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
    for (std::vector<std::string>::iterator i = conditions.begin (); i != conditions.end (); i++)
    {
        if (i != conditions.begin ()) file << ",\\";
        file << "\n\t\t\"" << escapeCode (splitCode (*i)) << "\"";
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
    for (std::vector<std::string>::iterator i = code.begin (); i != code.end (); i++)
    {
        if (i != code.begin ()) file << ",\\";
        file << "\n\t\t\"" << escapeCode (splitCode (*i)) << "\"";
    }
    
    // close array
    file << "]\n\n";
}

// write additional Python code
void DlgCompiler::writeCustomCode ()
{
    // constructor
    file << "\n\tdef __init__(self, p, n):"
         << "\n\t\tself.namespace = globals ()"
         << "\n\t\tself.the_player = p"
         << "\n\t\tself.the_npc = n\n";

    // custom constructor code
    if (dialogue->entry ()->ctor () != "")
        file << "\n" << splitCode (dialogue->entry ()->ctor (), 2) << "\n";
    
    // custom destructor code
    if (dialogue->entry ()->dtor () != "")
        file << "\n\tdef __del__(self):\n"
             << splitCode (dialogue->entry ()->dtor (), 2) << "\n";

    // custom methods
    if (dialogue->entry ()->methods () != "")
        file << "\n" << splitCode (dialogue->entry ()->methods (), 1) << "\n"; 
}

// replace '\n', '\t', '"' and the like with '\\n' '\\t' and '\"'
std::string DlgCompiler::escapeCode (std::string code)
{
    char c;
    
    for (unsigned int i = 0; i < code.length (); i++)
    {
        c = code[i];

        if (c == '"') code.insert (i++, "\\");
        else if (c == '\n') code.replace (i, 1, "\\n");
        else if (c == '\t') code.replace (i, 1, "    ");
    }
    
    return code;
}

// splits python code into individual lines
std::string DlgCompiler::splitCode (std::string code, int space)
{
    std::string new_code = "";
    unsigned int i = 0, j;

    code += '\n';

    while ((j = code.find ('\n', i)) < code.size ())
    {
        new_code.append (space, '\t');
        new_code += inflateCode (code.substr (i, j-i));
        new_code += "\n";
        i = ++j;
    }

    code.erase (code.end()-1);

    return new_code;
}

std::string DlgCompiler::inflateCode (std::string code)
{
    unsigned int i, begin = 0, pos, prefix, suffix;
    std::string token, stripped, last_op = "";
    bool is_local = true;

#ifdef _DEBUG_
    cout << ">>> " << code << endl;
#endif
    // replace the_npc/the_player with self.the_npc/self.the_player
    pos = code.find ("the_npc", 0);

    while (pos != code.npos)
    {
        if (pos < 5 || strncmp (code.substr (pos-5, pos).c_str(), "self.", 5))
        {
            code.insert (pos, "self.");
            pos += 5;
        }

        pos = code.find ("the_npc", pos+7);
    }

    pos = code.find ("the_player", 0);

    while (pos != code.npos)
    {
        if (pos < 5 || strncmp (code.substr (pos-5, pos).c_str(), "self.", 5))
        {
            code.insert (pos, "self.");
            pos += 5;
        }

        pos = code.find ("the_player", pos+10);
    }

    // scan the string from left to right
    for (pos = 0; pos < code.length (); pos++)
        for (i = 0; i < NUM_OPS; i++)
            // search for the leftmost operator from the current position
            if (!strncmp (code.substr (pos).c_str (), operators[i].c_str (),
                operators[i].length ()) || (i == NUM_OPS-1 && pos == code.length()-1))
            {
                // takes care of the rare situation when the last token
                // of the string is a variable in need of expanding
                if (pos == code.length()-1 && i == NUM_OPS-1) pos++;

                token = code.substr (begin, pos-begin);

                // strip leading and trailing whitespace
                for (prefix = 0; prefix < token.length() && token[prefix] == ' '; prefix++);
                for (suffix = token.length()-1; suffix >= 0 && token[suffix] == ' '; suffix--);
                stripped = token.substr (prefix, suffix-prefix+1);

                // have to be careful with textual operators and keywords
                if (i == BAND || i == BOR || i == NOT || i == RETURN ||
                    i == PASS || i == IF || i == ELIF || i == ELSE)
                {
                    if (pos > 0 && isalpha (code[pos-1]))
                        break;
                    if (pos < code.length()-operators[i].length()-1 &&
                        isalpha (code[pos+operators[i].length()]))
                        break;
                }

#ifdef _DEBUG_
                cout << "token = '" << stripped << "', operator = '" <<
                    operators[i] << "'\n" << flush;
#endif

                // skip functions and arrays
                if (i == LBRACKET || i == LBRACE)
                {
                    begin = pos + 1;
                    break;
                }

                // see whether we've got a variable and act accordingly
                if (getToken (stripped) == VARIABLE)
                {
                    // make sure we don't have a local variable
                    if (!is_local)
                    {
                        // assignment
                        if (i == ASSIGN)
                        {
                            code[pos] = ',';
                            code.insert (begin+suffix+1, "\"");
                            code.insert (begin+prefix, "set_val (\"");
                            code.append (")");
                            pos += 11;
                        }
                        else
                        {
                            code.insert (begin+suffix+1, "\")");
                            code.insert (begin+prefix, "get_val (\"");
                            pos += 12;
                        }
                    }

                    // variable left of '.'
                    if (i == ACCESS && last_op != ".")
                    {
                        // check whether we access the quest- or character array
                        if (dialogue->entry ()->isQuest (stripped))
                        {
                            code.insert (begin+prefix+stripped.length(), "\")");
                            code.insert (begin+prefix, "adonthell.gamedata_get_quest(\"");
                            pos += 32;
                            is_local = false;
                        }

                        if (dialogue->entry ()->isCharacter (stripped))
                        {
                            code.insert (begin+prefix+stripped.length(), "\")");
                            code.insert (begin+prefix, "adonthell.gamedata_get_character(\"");
                            pos += 36;
                            is_local = false;
                        }
                    }
                    else is_local = true;
                }

                // these are shortcuts for access to the character array, so
                // we handle them similar
                if (stripped == "the_npc" || stripped == "the_player")
                    is_local = false;

                // a trailing comma operator ends an expression
                if (i == COMMA)
                    is_local = true;

                // skip strings
                if (i == QUOT || i == SQUOT)
                    pos = code.find (operators[i], pos+1) - 1;

                // skip comments
                if (i == COMMENT)
                    pos = code.length ();

                last_op = operators[i];
                pos += operators[i].length ();
                begin = pos;
#ifdef _DEBUG_
                cout << code << endl;
                for (u_int32 j = 0; j < begin; j++) cout << " ";
                cout << "^\n";
#endif
                break;
            }

#ifdef _DEBUG_
    cout << "<<< " << code << "\n\n";
#endif
    return code;
}

// write the start of the dialogue
void DlgCompiler::writeStart ()
{
    // begin dialogue array
    file << "\t# -- (speaker, code, ((text, operation, condition), ...))"
         << "\n\tdlg = [\\\n"
         << "\t\t(None, -1, (";
    
    // write the "followers" (in this case the start nodes)
    if (!start.empty ())
    {
        std::vector<DlgNode*>::iterator i;

        for (i = start.begin (); i != start.end (); i++)
            writeFollower (*i);
    }
}

// write the actual dialogue data
void DlgCompiler::writeDialogue ()
{
    std::vector<DlgNode*> nodes = dialogue->getNodes ();
    std::vector<DlgNode*>::iterator i;
    DlgCircle *circle, *child;
    DlgCircleEntry *entry;
        
    for (i = nodes.begin (); i != nodes.end (); i++)
    {
        // nothing to be done for Arrows
        if ((*i)->type () == LINK) continue;
    
        circle = (DlgCircle *) (*i);
        
        // check the conditions of the circle's children
        checkConditions (circle);
        
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
        file << ", " << codeTable[circle->index ()] << ", (";
                
        // check whether the followers are valid
        checkFollowers (circle);
        
        // write all followers
        for (child = circle->child (FIRST); child != NULL; child = circle->child (NEXT))
            writeFollower (child);
    }
    
    // end of array
    file << "))]\n\n";
} 

// write a single follower
void DlgCompiler::writeFollower (DlgNode *node)
{
    file << "(" << node->index () << ", " << operationTable[node->index ()] 
         << ", " << conditionTable[node->index ()] << "), ";
}

// add node to the dialogue's start nodes
void DlgCompiler::addStart (DlgNode *node)
{
    std::vector<DlgNode*>::iterator i = start.begin ();

    // search the proper place for insertion
    while (i != start.end ())
    {
        if (*node < *(*i)) break;
        i++;
    }
    
    // insert
    start.insert (i, node);
}

// add a condition to the list of conditions
bool DlgCompiler::addCondition (DlgCircle *circle, int idx)
{
    std::string error, condition, cnd = circle->entry ()->condition ();
    bool retval = true;
    
    // see what kind of statement the condition is and get rid of the keyword
    switch (getKeyword (cnd))
    {
        case IF:
        {
            condition = cnd.substr (3);
            break;
        }
        
        case ELIF:
        {
            condition = cnd.substr (5);
            operationTable[idx] = 1;
            break;
        }
        
        case ELSE:
        {
            operationTable[idx] = 1;
            return true;        
        }
        
        default:
        {
            // a condition that doesn't start with any of the above is wrong
            error = "*** Error: Faulty condition\n    \"" + cnd;
            error += "\" of node\n    \"" + circle->entry ()->text ();
            error += "\"\n ";
        
            // add error to list
            if (DlgCmdline::compile) cout << error;
            else GuiError::console->add (error, circle);

            errors++;
            return false;
        }
    }
    
    // now get rid of the colon at the end of the condition
    if (condition[condition.size () - 1] == ':')
        condition.erase (condition.size () - 1);
    // if there is none, that's not too tragical, but report it anyway
    else
    {
        error = "*** Warning: Colon missing in condition\n    \"" + cnd;
        error += "\" of node\n    \"" + circle->entry ()->text ();
        error += "\"\n ";

        // add error to list
        if (DlgCmdline::compile) cout << error;
        else GuiError::console->add (error, circle);
        
        errors++;
        retval = false;
    }
    
    // now the condition is ready for addition to the condition vector
    for (std::vector<std::string>::iterator i = conditions.begin (); i != conditions.end (); i++)
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
void DlgCompiler::addCode (const std::string &cde, int idx)
{
    // see if code like this already exists
    for (std::vector<std::string>::iterator i = code.begin (); i != code.end (); i++)
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
    if (child == NULL) return 0;
    
    node_type type = child->type ();
    std::string error;
    
    // make sure that the followers are consistent
    for (; child != NULL; child = circle->child (NEXT))
    {
        if (type == child->type ()) continue;
        
        // that's an error
        if ((type == PLAYER && child->type () != PLAYER) ||
            (type != PLAYER && child->type () == PLAYER))
        {
            // compose error text
            error = "*** Error: Must not mix PLAYER and NPC nodes in";
            error += " children of node\n    \"" + circle->entry ()->text ();
            error += "\"\n ";
            
            // add error to the error console
            if (DlgCmdline::compile) cout << error;
            else GuiError::console->add (error, circle);
            
            errors++;
            break;
        }
    }

    return 1;
}

// check whether the children of given node have proper conditions
bool DlgCompiler::checkConditions (DlgCircle *circle)
{
    DlgCircle *child = circle->child (FIRST);
    if (child == NULL) return true;
    
    std::string error = "";
    bool retval = true;
    
    // get keyword of first child
    token k2, k1 = getKeyword (child->entry ()->condition ());
    
    for (; child != NULL; child = circle->child (NEXT), k1 = k2)
    {
        k2 = getKeyword (child->entry ()->condition ());
        if (k2 == NONE) continue;
        
        // 'elif' may only follow 'if' or 'elif'
        if (k2 == ELIF && (k1 != IF && k1 != ELIF))
            error = "*** Error: 'elif' without preceeding 'if' in node\n    \"";
        
        // 'else' may only follow 'if' or 'elif'
        else if (k2 == ELSE && (k1 != IF && k1 != ELIF))
            error = "*** Error: 'else' without preceeding 'if' in node\n    \"";

        // display error if there is any
        if (error != "")
        {
            error += child->entry ()->text ();
            error += "\"\n ";
            
            if (DlgCmdline::compile) cout << error;
            else GuiError::console->add (error, child);
            
            errors++;
            error = "";
            retval = false;
        }
    }
    
    return retval;
}

// get the keyword the statement begins with
token DlgCompiler::getKeyword (const std::string &statement)
{
    if (strncmp ("if ", statement.c_str (), 3) == 0)
        return IF;
    else if (strncmp ("elif ", statement.c_str (), 5) == 0)
        return ELIF;
    else if (strncmp ("else:", statement.c_str (), 5) == 0)
        return ELSE;    

    return NONE;
}

// splits a string into tokens
token DlgCompiler::getToken (const std::string &token)
{
    static unsigned int i;
    
    // No valid token
    if (token == "")
        return NONE;

    // Operator
    for (i = 0; i < NUM_OPS; i++)
        if (token == operators[i])
            return NONE;

    // Fixed: (i.e. something that never needs expanding)
    for (i = 0; i < NUM_FXD; i++)
        if (token == fixed[i])
            return FIXED;

    // Constant:
    if (isdigit (token[0]) || (token[0] == '-' && isdigit (token[token.length()-1])))
        return CONSTANT;

    // Variable:
    return VARIABLE;
}
