/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _FUNCTION_
#define _FUNCTION_

#include <map>

// Functions
enum
{
    _LET = 0,
    _THEN = 1,
    _IF = 2,
    _AND = 3,
    _OR = 4
};

// Operators
enum
{
    _ASSIGN = 0,
    _ADD = 1,
    _SUB = 2,
    _EQ = 3,
    _NEQ = 4,
    _LT = 5,
    _GT = 6
};

// data representing the function (operation (variable, value))
class function_data
{
public:
    u_int8 function;                // Function type
    u_int8 operation;               // Operation to execute upon variable and value
    char *variable;                 // lvalue of operation
    char *value;                    // rvalue of operation
};

// helper for the map<>
struct ltstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp (s1, s2) < 0;
    }
};

// The Function Dialog wrapper class  (model for all other GUI elements)
class function
{
public:
    function (DlgNode*);            // Thats indeed a constructor ;>
    ~function ();                   // destructor

    void add ();                    // Handlers
    void remove ();                 // for the
    void up ();                     // equally
    void down ();                   // labeled
    void ok ();                     // buttons.
    void select (s_int32);          // Change selection state of the list                 
    void set_function ();           // Change the function
    void set_operation ();          // Change the operation

    static char* fct_string[];      // String representation of functions
    static char* op_string[];       // String representation of operators
    static map<char*, s_int32, ltstr> vars;// Contains all used variables
    
private:
    void create_function_dialog (); // Create the GUI
    void set_variables ();          // Set the variables displayed in the Combobox
    void set_operators ();          // Change the accessible operations

    GtkWidget *function_dialog;     // The Dialogbox
    GtkWidget *function_list;       // The Listwidget containing assigned functions
    GtkWidget *variable;            // The ´Variable´-combobox
    GtkWidget *value;               // The ´Value´-combobox
    GtkWidget *operation;           // The ´Operation´-Dropdownlist
    GtkWidget *functions;            // The ´Function´-Dropdownlist
    GtkWidget *add_button;          // Add Function to node
    GtkWidget *remove_button;       // Remove function from node
    GtkWidget *up_button;           // Change the order of ...
    GtkWidget *down_button;         // ... the functions

    DlgNode *node;                  // The Arrow which is being edited


    s_int32 selection;              // Selected function in the list 
    s_int32 number;                 // Number of functions in list
    u_int32 thefunction;            // Function currently selected
    u_int32 theoperation;           // Operation currently selected
};

#endif // _FUNCTION_
