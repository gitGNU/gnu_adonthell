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
void create_code (string&);

// external functions
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int condlex();
extern YY_BUFFER_STATE cond_scan_string (const char*);
extern void cond_switch_to_buffer (YY_BUFFER_STATE);
extern void cond_delete_buffer (YY_BUFFER_STATE);

// some variables
vector<string> vars;
string err;
int error;
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
       | _IF _LPAREN comp _RPAREN   { $$ = string (1,BRANCH) + $3 + string (1,ENDIF); 
                                      create_code ($$); vars.clear (); }
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
    error = 0;
    err = "";

    // set the input buffer
    YY_BUFFER_STATE buffer = cond_scan_string (str);
    cond_switch_to_buffer (buffer);

    // start the bison parser
    condparse ();

    errormsg = err;

    // clean up
    cond_delete_buffer (buffer);

    return error;
}

void conderror(char *s)
{
    error = 1;
    err += string(s) + string ("near token") + condlval + string ("\n");
}

void create_code (string &prog)
{
    int i, j = vars.size ();
    unsigned char opcode;
    char regs = '0';
    char bools = '0';
    int cur_cmds = 0;
    
    static int num_cmds[255];
    static int index = 0;
    static int rec = 1;
        
    // Yeah, we're starting with the programs end first :)
    for (i = prog.size () - 1; i >= 0; i--)
    {
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
                cur_cmds++;
                
                break;
            }

            case LET:
            {
                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << "\n" << flush;
                prog.erase (prog.begin () + i, prog.begin () + i + 3);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);

                num_cmds[index]++;
                cur_cmds++;
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
                cur_cmds++;

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
                cur_cmds++;
                
                // Logic operation on two bools results in on bool, which means the
                // other is no longer needed and thus can be freed
                bools--;
                
                cout << "[" << rec << "] " << ops[opcode] << " " << vars[j] << " " << vars[j+1] << " local.bool" << bools << "\n";

                prog.replace (prog.begin () + i, prog.begin () + i + 3, 1, BOOL);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);
                vars.insert (vars.begin () + j, ("local.bool"+string(1, bools)));

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
                cout << "[" << rec << "] Branch " << vars[j] << " " << num_cmds[index]+1-cur_cmds << "\n";

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

    rec = 1;
}