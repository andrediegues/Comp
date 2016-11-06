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

\-?[0-9]+ { yylval.intValue = atoi(yytext); return INT; }

"+"	{return PLUS;}
"-" 	{return MINUS;}
"*" 	{return TIMES;}
"/" 	{return DIV;}
"%"	{return MOD;}
":"	{return COL;}
";"	{return SCOL;}
"("	{return LPAR;}
")"	{return RPAR;}
"<"	{return LT;}
">" 	{return GT;}
"<="	{return LTEQ;}
">="	{return GTEQ;}
"=="	{return EQ;}
"~="	{return NOTEQ;}
"="	{return ASSIGN;}

"true"	{return TRUE;}
"false"	{return FALSE;}
"disp" 	{return OUT;}
"input"	{return IN;}
"if"	{return IF;}
"else"	{return ELSE;}
"elseif" {return ELSEIF;}
"end"	{return END;}
"while"	{return WHILE;}
"for"	{return FOR;}


\-?[a-zA-Z]+ { yylval.varValue = strdup(yytext); return VAR; }

.  	{ yyerror("unexpected character"); }

%%

