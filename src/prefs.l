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
#include "prefs.h"

#define YY_DECL int parse_adonthellrc (int &mynum, string &mytext)
%}

digit  [0-9]
num    {digit}+

%x text
%option noyywrap nounput never-interactive prefix="prefs"

%%

"#".*[\n]? ;        /* Eat up comments */

Default             return PREFS_DEFAULT;
Section             return PREFS_SECTION;
End                 return PREFS_END;
Data                return PREFS_DATA;
Map                 return PREFS_MAP;
Screen-resolution   return PREFS_SCREEN_RESOLUTION;
Screen-mode         return PREFS_SCREEN_MODE;
Audio-channels      return PREFS_AUDIO_CHANNELS;
Audio-resolution    return PREFS_AUDIO_RESOLUTION;
Audio-sample-rate   return PREFS_AUDIO_SAMPLE_RATE;
Audio-volume        return PREFS_AUDIO_VOLUME;

\[                  mytext = ""; BEGIN(text);
<text>\]            BEGIN(INITIAL); return PREFS_STR;
<text>[^\]]+        mytext += yytext;

{num}               mynum = atoi (yytext); return PREFS_NUM;

[ \t\n]+ ;
.                   return PREFS_UNKNOWN;
