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
 * @file dlg_compiler.h
 *
 * @author Kai Sterker
 * @brief The dialogue compiler. 
 */

#ifndef DLG_COMPILER_H
#define DLG_COMPILER_H

#include <vector>
#include <fstream.h>
#include "dlg_module.h"
#include "dlg_circle.h"

/**
 * Python keywords
 */ 
enum keyword
{
    NONE    = 0,
    IF      = 1,
    ELIF    = 2,
    ELSE    = 3
};

/**
 * It transforms the dialogue into the Python script needed by
 * the Dialogue Engine. 
 *
 * The Engine itself consists of two parts: 
 * - the dialogue base class defined in the dialogue.py module. It 
 *   contains executable code shared by all dialogues.
 * - the C++ part of the Engine uses the methods the Python dialogue
 *   class provides to step through the dialogue and retrieve the 
 *   respective text.
 *
 * The main reason for this architecture is to keep the individual 
 * dialogue scripts as small as possible. Therefore, the scripts
 * created by the compiler contain mainly data and little or no code.
 */
class DlgCompiler
{
public:
    /**
     * Instantiate the compiler.
     * @param module Module to be compiled
     */
    DlgCompiler (DlgModule *module);
    /**
     * Dtor.
     */
    ~DlgCompiler ();
    
    /**
     * Compile the module passed to DlgCompiler
     */
    void run ();
    
private:
    void writeHeader (const string &theClass);
    void writeText ();
    void writeCode ();
    void writeConditions ();
    void writeStart ();
    void writeDialogue ();
    void writeFollower (DlgNode *node);
    
    void addStart (DlgNode *node);
    void addCode (const string &cde, int index);
    bool addCondition (DlgCircle *circle, int index);
    
    int checkFollowers (DlgCircle *node);
    bool checkConditions (DlgCircle* node);
    
    keyword getKeyword (const string &statement);
    
    ofstream file;
    DlgModule *dialogue;            // The dialogue to be compiled    
    
    vector<DlgNode*> start;         // Start nodes of the dialogue
    vector<string> code;            // Temporary storage for all code
    vector<string> conditions;      // Temporary storage for all conditions

    int errors;                     // number of errors in dialogue
    
    int *codeTable;                 // Mapping between nodes and code
    int *conditionTable;            // Mapping between nodes and conditions
    int *operationTable;            // Mapping between nodes and condition type
};

#endif // DLG_COMPILER_H
