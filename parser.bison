// Tokens
%token INT
%token PLUS
%token TIMES
%token DIV
%token VAR
%token MOD
%token LPAR RPAR
%token ASSIGN LT GT LTEQ GTEQ EQ NOTEQ
%token WHILE FOR IF ELSE ELSEIF END

// Operator associativity & precedence
%left ASSIGN
%left EQ NOTEQ
%left LT GT LTEQ GTEQ
%left PLUS MINUS
%left TIMES DIV
%left MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  Expr* exprValue;
  char *varValue;
}

%type <intValue> INT
%type <exprValue> expr
%type <varValue> VAR
//%type cmd

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
Expr* root;
}

%%
program: expr { root = $1; }

expr: 
  INT { 
    $$ = ast_integer($1); 
  }
  | expr PLUS expr	{$$ = ast_operation(PLUS , $1, $3);}
  | expr MINUS expr 	{$$ = ast_operation(MINUS,$1,$3);}     
  | expr TIMES expr 	{$$ = ast_operation(TIMES, $1, $3);}
  | expr DIV expr 	{$$ = ast_operation(DIV	 , $1, $3);}
  | expr MOD expr 	{$$ = ast_operation(MOD	 , $1, $3);}
  | expr LT expr	{$$ = ast_operation(LT, $1, $3);}
  | expr GT expr	{$$ = ast_operation(GT, $1, $3);}
  | expr EQ expr	{$$ = ast_operation(EQ, $1, $3);}
  | expr NOTEQ expr	{$$ = ast_operation(NOTEQ, $1, $3);}
  | expr LTEQ expr	{$$ = ast_operation(LTEQ, $1, $3);}
  | expr GTEQ expr	{$$ = ast_operation(GTEQ, $1, $3);}
  | LPAR expr RPAR	{$$ = $2;}

  //VAR {$$ = ast_variable($1);}
  ;
 /* cmd: VAR ASSIGN expr {$$ = ast_assign($1, $3);}
       | IF expr cmd_list END
       | WHILE expr cmd_list END
       | FOR expr cmd_list END
       | IF expr cmd_list ELSEIF expr cmd_list END
       

cmd_list: cmd ';'
	  cmd ';' cmd_list*/
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

