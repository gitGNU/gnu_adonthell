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

#include <math.h>
#include <string>
#include "dlgnode.h"

#define YY_DECL int parse_dlgfile (string &mytext, int &mynum)
%}

digit  [0-9]
num    {digit}+

%x text
%option noyywrap nounput never-interactive prefix="loadlg"

%%

"#".*[\n]? ;    /* Eat up comments */

Circle          return LOAD_CIRCLE;
Arrow           return LOAD_ARROW;
End             return LOAD_END;
Type            return LOAD_TYPE;
Prev            return LOAD_PREV;
Next            return LOAD_NEXT;
Link            return LOAD_LINK;
Pos             return LOAD_POS;
Note            return LOAD_NOTE;
Text            return LOAD_TEXT;
Cond            return LOAD_COND;
Vars            return LOAD_VARS;

\[              mytext = ""; BEGIN(text);
<text>\]        BEGIN(INITIAL); return LOAD_STR;
<text>[^\]]+    mytext += yytext;

{num}           mynum = atoi (yytext); return LOAD_NUM;

[ \t\n]+ ;
.               return LOAD_UNKNOWN;
