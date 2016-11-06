// Tokens
%token INT
%token PLUS
%token MINUS
%token TIMES
%token DIV
%token VAR
%token MOD
%token LPAR RPAR
%token ASSIGN LT GT LTEQ GTEQ EQ NOTEQ TRUE FALSE
%token WHILE FOR IF ELSE ELSEIF END
%token IN OUT COL SCOL

// Operator associativity & precedence
%left ASSIGN
%nonassoc EQ NOTEQ LT GT LTEQ GTEQ
%left PLUS MINUS
%left TIMES DIV MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  Expr* exprValue;
  char* varValue;
  cmd_list* list;
  cmd* command;
}

%type <intValue> INT
%type <exprValue> expr
%type <varValue> VAR
%type <list> cmdlist
%type <command> cmds

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
cmd_list* root;
}

%%
program: cmdlist { root = $1; } 

cmdlist: cmds {$$ = ast_list($1, NULL);}
       | cmds cmdlist { $$ = ast_list($1, $2);}
  
cmds:  VAR ASSIGN expr SCOL		{$$ = ast_assign($1, $3);}
       | VAR ASSIGN expr		{$$ = ast_assign($1, $3);} 
       | IF expr cmdlist END		{$$ = ast_if($2, $3);}
       | ELSEIF expr cmdlist END	{$$ = ast_elseif($2, $3);}
       | ELSE cmdlist END     		{$$ = ast_else($2);}
       | WHILE expr cmdlist END		{$$ = ast_while($2, $3);}
       | FOR cmds COL expr cmdlist END {$$ = ast_for($2, $4, $5);}
       | OUT LPAR expr RPAR SCOL	{$$ = ast_out($3);}
       | VAR ASSIGN IN LPAR RPAR SCOL {$$ = ast_in($1);}
expr: 
  INT			{$$ = ast_integer($1);}  
  | TRUE      		{$$ = ast_integer(1);}
  | FALSE		{$$ = ast_integer(0);}
  | VAR 		{$$ = ast_variable($1);}
  | expr PLUS expr	{$$ = ast_operation(PLUS, $1, $3);}
  | expr MINUS expr 	{$$ = ast_operation(MINUS,$1,$3);}     
  | expr TIMES expr 	{$$ = ast_operation(TIMES, $1, $3);}
  | expr DIV expr 	{$$ = ast_operation(DIV, $1, $3);}
  | expr MOD expr 	{$$ = ast_operation(MOD, $1, $3);}
  | expr LT expr	{$$ = ast_operation(LT, $1, $3);}
  | expr GT expr	{$$ = ast_operation(GT, $1, $3);}
  | expr EQ expr	{$$ = ast_operation(EQ, $1, $3);}
  | expr NOTEQ expr	{$$ = ast_operation(NOTEQ, $1, $3);}
  | expr LTEQ expr	{$$ = ast_operation(LTEQ, $1, $3);}
  | expr GTEQ expr	{$$ = ast_operation(GTEQ, $1, $3);}
  | LPAR expr RPAR	{$$ = $2;}
  ;

%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

