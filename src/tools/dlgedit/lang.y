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

#define YYSTYPE string

int yyerror(char *);
int yylex();
void create_code (string&);

vector<string> vars;

char* ops[] = { "", "", "", "", "", "", "Let", "Add", "Sub", "Mul", "Div",
                "Eq", "Neq", "Lt", "Leq", "Gt", "Geq", "And", "Or" };
%}

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
       | input stat                 { create_code ($2); vars.clear (); }
       | error                      { yyerrok; yyclearin; } 
;

stat:     assign                    { $$ = $1; }
	   | if_stat	                { $$ = $1; }
;

assign:   _ID _ASSIGN expr _SEMICOLON  { $$ = string(1, LET) + $3 + string(1, ID); vars.push_back ($1); }
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

expr:     val                       { $$ = $1; }
        | expr _ADD expr            { $$ = string(1, ADD) + $1 + $3; }
        | expr _SUB expr            { $$ = string(1, SUB) + $1 + $3; } 
        | expr _MUL expr            { $$ = string(1, MUL) + $1 + $3; }
        | expr _DIV expr            { $$ = string(1, DIV) + $1 + $3; }       
        | _LPAREN expr _RPAREN      { $$ = $2; }
;

if_stat:  _IF _LPAREN comp _RPAREN block    { $$ = string (1,BRANCH) + $3 + $5 + string (1,ENDIF); }
	    | _IF _LPAREN comp _RPAREN block _ELSE block  { $$ = string (1,BRANCH) + $3 + $5 + string (1,JMP) + $7 + string (1,ENDIF); }
;

block:    _LBRACE assign_list _RBRACE   { $$ = $2; }
        | stat                      { $$ = $1; }
;

assign_list: assign_list stat       { $$ = $1 + $2; }
        | stat                      { $$ = $1; }
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

%%

int yyerror(char *s)
{
  printf("%s near token %s\n", s, yylval.c_str ());
}

int main()
{
  yyparse();
}

// Transform the scanned and parsed code into the ASM-like script understandable 
// by the interpreter. Since the code isn't quite in the right order, we have to
// use a bit of recursion :)
//
// As an example, the code:
//
// if (e - 2 < 5 - a) 
// {
//     x = b + c * d * (e - (f/g));
//     y = h;
// }
// else
//     z = 0;
//
// results in the following _p_rogram and _a_rgument stacks:
//
// p: branch or lt sub id num sub num id then let add id mul mul id id sub id
//    div id id id let id id else let num id
// a: e 2 5 a b c d e f g x h y 0 z
//
// out of which we have to make: (with regx being temporary register x)
//
// sub 5 a reg1
// sub e 2 reg2
// lt reg2 reg1 bool1
// branch bool1 9
//
// div f g reg1
// sub e reg1 reg1
// mul c d reg2
// mul reg2 reg1 reg1
// add b reg1 reg1
// let reg1 x
//
// let h y
// jmp 2
//
// let 0 z
//
// Ok, now we know what to do, so lets go :)
void create_code (string &prog)
{
    int i, j = vars.size ();
    unsigned char opcode;
    char regs = '0';
    char bools = '0';
    
    static int num_cmds[255];
    static int index = 0;
    static int rec = 1;
        
    // Yeah, we're starting with the programs end first :)
    for (i = prog.size () - 1; i >= 0; i--)
    {
#ifdef _DEBUG_    
        // prints program and argument stack
        if (prog[i] > 0)
        {
            cout << "*** ";
            copy(prog.begin(), prog.end(), ostream_iterator<int>(cout, " "));
            cout << "\n*** ";
            copy(vars.begin(), vars.end(), ostream_iterator<string>(cout, " "));
            cout << endl;
       }
#endif // _DEBUG_
       
        switch (opcode = prog[i])
        {
            case ID:
            case NUM:
            case REG:
            case BOOL:
            {
                j--;
                continue;
            }

            case ADD:
            case SUB:
            case MUL:
            case DIV:
            {
                // if both arguments are registers, one of them is no longer needed afterwards
                if (prog[i+1] == char(REG) && prog[i+2] == char(REG)) regs--;

                // store result into a new register if both arguments are immediate ones
                if (prog[i+1] != char(REG) && prog[i+2] != char(REG)) regs++;

                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " local.reg" << regs << "\n";
                prog.replace (prog.begin () + i, prog.begin () + i + 3, 1, REG);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);
                vars.insert (vars.begin () + j, ("local.reg"+string(1, regs)));

                i = prog.size ();
                j = vars.size ();

                num_cmds[index]++;
                
                break;
            }

            case LET:
            {
                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << "\n" << flush;
                prog.erase (prog.begin () + i, prog.begin () + i + 3);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);

                num_cmds[index]++;
                rec++;

                // recursion to bring the resulting script into the right order
                create_code (prog);

                i = prog.size ();
                j = vars.size ();

                break;
            }

            case EQ:
            case NEQ:
            case LT:
            case LEQ:
            case GT:
            case GEQ:
            {
                // Comparison of two integer args results in boolean result
                bools++;
                num_cmds[index]++;

                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " local.bool" << bools << "\n";
                
                prog.replace (prog.begin () + i, prog.begin () + i + 3, 1, BOOL);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);
                vars.insert (vars.begin () + j, ("local.bool"+string(1, bools)));

                i = prog.size ();
                j = vars.size ();

                break;
            }

            case AND:
            case OR:
            {
                num_cmds[index]++;
                // Logic operation on two bools results in on bool, which means the
                // other is no longer needed and thus can be freed
                bools--;
                
                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " " << vars[j+1] << " bool" << bools << "\n";

                prog.replace (prog.begin () + i, prog.begin () + i + 3, 1, BOOL);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);
                vars.insert (vars.begin () + j, ("bool"+string(1, bools)));

                i = prog.size ();
                j = vars.size ();

                break;
            }

            case JMP:
            {
                cout << "[" << rec << "] Jmp " << num_cmds[index]+1 << "\n";

                prog.erase (prog.begin () + i, prog.begin () + i + 1);
                
                rec++;
                num_cmds[index-1] += num_cmds[index];
                num_cmds[index] = 1;

                create_code (prog);
                
                i = prog.size ();
            
                break;
           }
            
            case BRANCH:
            {
                cout << "[" << rec << "] Branch " << vars[j] << " " << num_cmds[index] << "\n";

                prog.erase (prog.begin () + i, prog.begin () + i + 2);
                vars.erase (vars.begin () + j, vars.begin () + j + 1);            

                rec++;
                num_cmds[index-1] += num_cmds[index] + 1;
                index--;
                
                create_code (prog);
                
                i = prog.size ();
                j = vars.size ();

                break;
            }

            // Increase the 'stackpointer' for if - else - statements
            case ENDIF:
            {
                prog.erase (prog.begin () + i, prog.begin () + i + 1);

                index++;
                num_cmds[index] = 0;
                
                i = prog.size ();
            
                break;
            }
        }
    } 
}