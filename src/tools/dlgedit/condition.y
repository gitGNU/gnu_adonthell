%{
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

#include <string>
#include <vector>
#include "../../interpreter.h"
#include "../../commands.h"

#define YYSTYPE string

// local functions
int cond_compile (const char*, string&, vector<command*>&);
void conderror(char *);
void create_code (string&, vector<command*>&);

// external functions
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int condlex();
extern YY_BUFFER_STATE cond_scan_string (const char*);
extern void cond_switch_to_buffer (YY_BUFFER_STATE);
extern void cond_delete_buffer (YY_BUFFER_STATE);
extern void create_code (string&);
extern vector<string> vars;

// some variables
string c_err;
int c_error;
vector<command*> c_scrpt;
%}

%token _ID
%token _IF
%token _NUM
%token _LPAREN _RPAREN
%nonassoc _EQ _NEQ _LT _GT _LEQ _GEQ
%left _ADD _SUB
%left _MUL _DIV
%left _NEG
%left _OR
%left _AND

%start input

%%

input:   /* empty */
       | _IF _LPAREN comp _RPAREN   { $$ = string (1,BRANCH) + $3 + string (1,ENDIF); 
                                      create_code ($$, c_scrpt); vars.clear (); }
       | error                      { yyerrok; yyclearin; } 
;

comp:     comp _AND comp            { $$ = string(1, AND) + $1 + $3; }
        | comp _OR comp             { $$ = string(1, OR) + $1 + $3; }
        | _LPAREN comp _RPAREN      { $$ = $2; }
        | expr _EQ expr             { $$ = string(1, EQ) + $1 + $3; }
        | expr _NEQ expr            { $$ = string(1, NEQ) + $1 + $3; }
        | expr _LT expr             { $$ = string(1, LT) + $1 + $3; }
        | expr _LEQ expr            { $$ = string(1, LEQ) + $1 + $3; }
        | expr _GT expr             { $$ = string(1, GT) + $1 + $3; }
        | expr _GEQ expr            { $$ = string(1, GEQ) + $1 + $3; }
;

expr:     val                       { $$ = $1; }
        | expr _ADD expr            { $$ = string(1, ADD) + $1 + $3; }
        | expr _SUB expr            { $$ = string(1, SUB) + $1 + $3; } 
        | expr _MUL expr            { $$ = string(1, MUL) + $1 + $3; }
        | expr _DIV expr            { $$ = string(1, DIV) + $1 + $3; }       
        | _LPAREN expr _RPAREN      { $$ = $2; }
;

val:      _NUM                      { $$ = NUM; vars.push_back ($1); }
        | _ID                       { $$ = ID; vars.push_back ($1); }
        | _SUB val %prec _NEG       { if ($2[0] == char(ID)) {
                                        $$ = string(1, SUB) + string(1, NUM) + $2; vars.insert ((vars.begin () + vars.size () - 1), "0");
                                      } else {
                                        $$ = $2; vars[vars.size () - 1] = "-" + vars[vars.size () - 1];
                                      }
                                    }
;

%%

int cond_compile (const char *str, string &errormsg, vector<command*> &script)
{
    c_error = 0;
    c_err = "";

    // set the input buffer
    YY_BUFFER_STATE buffer = cond_scan_string (str);
    cond_switch_to_buffer (buffer);

    // start the bison parser
    condparse ();

    errormsg = c_err;
    script = c_scrpt;

    // clean up
    cond_delete_buffer (buffer);

    return c_error;
}

void conderror(char *s)
{
    c_error = 1;
    c_err += string(s) + string (" near token ") + condlval + string ("\n");
}