%{
/*
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
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

#define YY_DECL int parse_cfgfile (std::string &mytext, int &mynum)
%}

digit  [0-9]
num    -?{digit}+

%x text
%option noyywrap nounput never-interactive prefix="loadcfg"

%%

"#".*[\n]? ;    /* Eat up comments */

File            return LOAD_FILE;
Project         return LOAD_PROJECT;
BaseDir         return LOAD_BASE_DIR;
End             return LOAD_END;

\[              mytext = ""; BEGIN(text);
<text>\]        BEGIN(INITIAL); return LOAD_STR;
<text>[^\]]+    mytext += yytext;

{num}           mynum = atoi (yytext); return LOAD_NUM;

[ \t\n]+ ;      /* Eat whitespace */
.               return LOAD_UNKNOWN;
