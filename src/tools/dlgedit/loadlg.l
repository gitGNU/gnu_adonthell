%{
/*
   Copyright (C) 2000/2002/2003 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <string>
#include "dlg_types.h"

#define YY_DECL int parse_dlgfile (std::string &mytext, int &mynum)

// definitions for using 'includes'
#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

// switch to another file for input
void parser_switch_input ()
{
    include_stack[include_stack_ptr++] = YY_CURRENT_BUFFER;
    yy_switch_to_buffer (yy_create_buffer (yyin, YY_BUF_SIZE));
}

// return to previous file
void parser_restore_input ()
{
    yy_delete_buffer (YY_CURRENT_BUFFER);
    yy_switch_to_buffer (include_stack[include_stack_ptr]);
}
%}

digit  [0-9]
num    -?{digit}+

%x text module
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
Func            return LOAD_FUNC;
NPC             return LOAD_NPC;
Name            return LOAD_NAME;
Race            return LOAD_RACE;
Gender          return LOAD_GENDER;
Loop            return LOAD_LOOP;
Proj            return LOAD_PROJECT;
Inc             return LOAD_IMPORTS;
Dtor            return LOAD_DTOR;
Ctor            return LOAD_CTOR;
Id              return LOAD_ID;

Module          mytext = ""; BEGIN(module); return LOAD_MODULE;

<module>[ \t]   /* Eat whitespace */
<module>[^ \t\n]+ {
                mytext += yytext;
                BEGIN(INITIAL);

                if (include_stack_ptr >= MAX_INCLUDE_DEPTH)
                {
                    fprintf (stderr, "Modules nested too deeply\n");
                    return LOAD_UNKNOWN;
                }
                
                return LOAD_STR;
                }

<<EOF>>         {
                if (--include_stack_ptr < 0)
                {
                    include_stack_ptr = 0;
                    yyterminate ();
                }
                else
                {
                    parser_restore_input ();
                    return 0;
                }
                }
             
\xa7            mytext = ""; BEGIN(text);
<text>\xa7      BEGIN(INITIAL); return LOAD_STR;
<text>[^\xa7]+  mytext += yytext;

{num}           mynum = atoi (yytext); return LOAD_NUM;

[ \t\n]+ ;
.               return LOAD_UNKNOWN;
