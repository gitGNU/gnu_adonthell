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

#include <iostream.h>
#include <string>
#include <vector>
#include "../../commands.h"
#include "../../interpreter.h"

#define YYSTYPE string

// local functions
int vars_compile (const char*, string&, vector<command*>&);
void varserror(char *);
void create_code (string&);

// external functions
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int varslex();
extern YY_BUFFER_STATE vars_scan_string (const char*);
extern void vars_switch_to_buffer (YY_BUFFER_STATE);
extern void vars_delete_buffer (YY_BUFFER_STATE);
extern void create_code (string&, vector<command*>&);
extern vector<string> vars;

// some variables
string v_err;
int v_error;
vector<command*> v_scrpt;
%}

%expect 1

%token _ID
%token _IF
%token _ELSE
%token _NUM
%token _LPAREN _RPAREN
%token _LBRACE _RBRACE
%token _SEMICOLON
%nonassoc _EQ _NEQ _LT _GT _LEQ _GEQ
%right _ASSIGN
%left _ADD _SUB
%left _MUL _DIV
%left _NEG
%left _OR
%left _AND

%start input

%%

input:   /* empty */
       | input stat                 { create_code ($2, v_scrpt); vars.clear (); }
       | error                      { yyerrok; yyclearin; } 
;

stat:     assign                    { $$ = $1; }
	   | if_stat	                { $$ = $1; }
;

assign:   _ID _ASSIGN expr _SEMICOLON  { $$ = string(1, LET) + $3 + string(1, ID); vars.push_back ($1); }
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

if_stat:  _IF _LPAREN comp _RPAREN block    { $$ = string (1,BRANCH) + $3 + $5 + string (1,ENDIF); }
	    | _IF _LPAREN comp _RPAREN block _ELSE block  { $$ = string (1,BRANCH) + $3 + $5 + string (1,JMP) + $7 + string (1,ENDIF); }
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

block:    _LBRACE assign_list _RBRACE   { $$ = $2; }
        | stat                      { $$ = $1; }
;

assign_list: assign_list stat       { $$ = $1 + $2; }
        | stat                      { $$ = $1; }
;

%%

void varserror(char *s)
{
    v_error = 1;
    v_err += string(s) + string (" near token ") + varslval + string ("\n");
}

int vars_compile (const char *str, string &errormsg, vector<command*> &script)
{
    v_error = 0;
    v_err = "";

    // set the input buffer
    YY_BUFFER_STATE buffer = vars_scan_string (str);
    vars_switch_to_buffer (buffer);

    // start the bison parser
    varsparse ();

    errormsg = v_err;
    script = v_scrpt;

    // clean up
    vars_delete_buffer (buffer);

    return v_error;
}