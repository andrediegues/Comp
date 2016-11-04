%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%

"+"	{return PLUS;}
"-" 	{return MINUS;}
"*" 	{return TIMES;}
"/" 	{return DIV;}
"%"	{return MOD;}
"("	{return LPAR;}
")"	{return RPAR;}
"<"	{return LT;}
">" 	{return GT;}
"<="	{return LTEQ;}
">="	{return GTEQ;}
"=="	{return EQ;}
"~="	{return NOTEQ;}
"="	{return ASSIGN;}
":"	{return COL;}
";"	{return SCOL;}

"disp" 	{return OUT;}
"input"	{return IN;}
"if"	{return IF;}
"else"	{return ELSE;}
"elseif" {return ELSEIF;}
"end"	{return END;}
"while"	{return WHILE;}
"for"	{return FOR;}
.  	{ yyerror("unexpected character"); }


[# \t\n]+ {  }
[a-zA-Z_][a-zA-Z0-9_]* {
   yylval.varValue = strdup(yytext);
   return VAR;
}
[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
%%

