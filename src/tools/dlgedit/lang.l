%{
#include <string>
#include "lang.tab.h"
%}

letter [A-Za-z_]
digit  [0-9]
ident  {letter}({letter}|{digit})*
num    {digit}+

%%
[ \t\n]+    ;           // Whitespace
"//".*\n    ;           // C++ style comments
"/*"(.|[\n])*"*/"   ;   // C style comments

if          { return _IF; }
else        { return _ELSE; }
{ident}     { yylval = yytext; return _ID; }
{num}       { yylval = yytext; return _NUM; }
"("         { return _LPAREN; }
")"         { return _RPAREN; } 
"=="        { return _EQ; }
"!="        { return _NEQ; }
"<="        { return _LEQ; }
">="        { return _GEQ; }
">"         { return _GT; }
"<"         { return _LT; }
"="         { return _ASSIGN; }
"+"         { return _ADD; }
"-"         { return _SUB; }
"*"         { return _MUL; }
"/"         { return _DIV; }
"&&"        { return _AND; }
"||"        { return _OR; }
";"         { return _SEMICOLON; }

.           { printf("Illegal character.\n"); return yytext[0]; }
