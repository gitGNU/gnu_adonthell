%{
#include <iostream.h>
#include <string>
#include <vector>
#include "../../commands.h"

#define YYSTYPE string

int yyerror(char *);
int yylex();
void create_code (string);

vector<string> vars;

char* ops[] = { "", "", "", "", "", "", "Let", "Add", "Sub", "Mul", "Div",
                "Eq", "Neq", "Lt", "Leq", "Gt", "Geq", "And", "Or" };
%}

%token _ID
%token _IF
%token _ELSE
%token _NUM
%token _LPAREN
%token _RPAREN
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

if_stat:  _IF _LPAREN comp _RPAREN assign    { $$ = $3 + string (1,THEN) + $5; }
	    | _IF _LPAREN comp _RPAREN assign _ELSE assign  { $$ = $3 + string(1,THEN) + $5 + string(1,ELSE) + $7; }
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

int get_concat (string &prog, int index)
{
    int i, opcode;
    
    for (i = index; i >= 0; i--)
    {
        opcode = prog[i];
          
        if (opcode == AND || opcode == OR)
        {
            prog.erase (prog.begin () + i);
            return opcode;
        }
    }
    
    return 0;
}

void create_code (string prog)
{
    int i, j = vars.size (), k = 0;
    int code[3] = { 0, 0, 0 };
    int numvals[3] = { 0, 0, 0 };
    char regs = '0';
    int then_length, else_length = 0;
    int num_cmds = 0;
    unsigned char opcode;
    
    for (i = 0; i < prog.size (); i++)
    {
        switch (opcode = prog[i])
        {
            case ID:
            case NUM:
            {
                numvals[k]++;
                code[k]++;
                break;
            }

            case THEN:
            case ELSE:
            {
                k++;
                break;
            }

            default:
            {
                code[k]++;
                break;
            }
        } 
    }

//    cout << "then: " << code[1] << "/" << numvals[1] << " else:" << code[2] << "/" << numvals[2] << "\n";

//    for (k = 0; k < 3; k++)
//    {
//        j = numvals[k];
        
    for (i = prog.size () - 1; i >= 0; i--)
    {
       
        switch (opcode = prog[i])
        {
            case ID:
            case NUM:
            case REG:
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
                if (prog[i+1] == char(REG) && prog[i+2] == char(REG) && regs > '0') regs--;

                // store result into a new register if both arguments are immediate ones
                if (prog[i+1] != char(REG) && prog[i+2] != char(REG)) regs++;

                cout << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " reg" << regs << "\n";
                prog.replace (prog.begin () + i, prog.begin () + i + 3, 1, REG);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);
                vars.insert (vars.begin () + j, ("reg"+string(1, regs)));

                i = prog.size ();
                j = vars.size ();

                num_cmds++;
                
                break;
            }

            case LET:
            {
                cout << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << "\n" << flush;
                prog.erase (prog.begin () + i, prog.begin () + i + 3);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);

                i = prog.size ();
                j = vars.size ();
                regs = '0';

                num_cmds++;
                
                break;
            }

            case THEN:
            {
                then_length = num_cmds;
                num_cmds = 0;
                prog.erase (prog.begin () + i);
                continue;
            }

            case ELSE:
            {
                else_length = num_cmds;
                num_cmds = 0;
                prog.erase (prog.begin () + i);
                continue;
            }

            case EQ:
            case NEQ:
            case LT:
            case LEQ:
            case GT:
            case GEQ:
            {
                // Look wether AND or OR follows
                switch (get_concat (prog, i))
                {
                    case AND:
                    {
                        cout << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " else\n" << flush;
                        i--;
                        break;
                    }
                    case OR:
                    {
                        cout << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " 2\nJmp then\n" << flush;
                        i--;
                        break;
                    }
                    default:
                    {
                        cout << ops[opcode] << " " << vars[j] << " "<< vars[j+1] << " else\n" << flush;
                        break;
                    }
                }
                
                prog.erase (prog.begin () + i, prog.begin () + i + 3);
                vars.erase (vars.begin () + j, vars.begin () + j + 2);

                i = prog.size ();
                j = vars.size ();
                regs = '0';
            }
        }

        // prints program and argument stack
        cout << "*** ";
        copy(prog.begin(), prog.end(), ostream_iterator<int>(cout, " "));
        cout << "\n*** ";
        copy(vars.begin(), vars.end(), ostream_iterator<string>(cout, " "));
        cout << endl;

//    }
    }

    cout << "*** then " << then_length << "\n*** else " << else_length << "\n";  
}