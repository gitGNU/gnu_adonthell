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

#define YYSTYPE string

#include <string>
#include "cond.tab.h"
%}


letter [A-Za-z_.]
digit  [0-9]
ident  {letter}({letter}|{digit})*
num    {digit}+

%option noyywrap nounput never-interactive prefix="cond"

%%

[ \t\n]+    ;           // Whitespace
"//".*[\n]? ;           /* C++ style comments (the newline has to be optional, else a
                        comment in the last line wouldn't be recognized correctly) */
"/*"(.|[\n])*"*/"   ;   // C style comments

if          { return _IF; }
{ident}     { condlval = condtext; return _ID; }
{num}       { condlval = condtext; return _NUM; }
"("         { return _LPAREN; }
")"         { return _RPAREN; } 
"=="        { return _EQ; }
"!="        { return _NEQ; }
"<="        { return _LEQ; }
">="        { return _GEQ; }
">"         { return _GT; }
"<"         { return _LT; }
"+"         { return _ADD; }
"-"         { return _SUB; }
"*"         { return _MUL; }
"/"         { return _DIV; }
"&&"        { return _AND; }
"||"        { return _OR; }

.           { printf("Illegal character.\n"); return condtext[0]; }
