%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }
[a-z][a-z0-9]* {return VAR;}
\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
"+"	{return PLUS;}
"-" 	{return MINUS;}
"*" 	{return TIMES;}
"/" 	{return DIV;}
"%"	{return MOD;}
.  	{ yyerror("unexpected character"); }
%%

