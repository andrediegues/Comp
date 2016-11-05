// AST constructor functions

#include <stdlib.h> // for malloc
#include <string.h>
#include "ast.h" // AST header

Expr* ast_integer(int v) {
  Expr* exp = (Expr*) malloc(sizeof(Expr));
  exp->kind = E_INTEGER;
  exp->attr.value = v;
  return exp;
}

Expr* ast_operation(int operator, Expr* left, Expr* right) {
  Expr* exp = (Expr*) malloc(sizeof(Expr));
  exp->kind = E_OPERATION;
  exp->attr.op.operator = operator;
  exp->attr.op.left = left;
  exp->attr.op.right = right;
  return exp;
}

Expr* ast_variable(char* name){
  Expr* exp = (Expr*) malloc(sizeof(Expr));
  exp -> kind = E_VAR;
  exp -> attr.name = name;
  return exp;
}

cmd* ast_assign(char* var, Expr* expr){  //COMMAND -> ATRRIBUTION
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com->kind = C_ASSIGN;
  com->comm.assign.var=strdup(var);
  com->comm.assign.expr=expr;
  return com;
}

cmd_list* ast_list(cmd* command, cmd_list* next) { //LIST
  cmd_list* com = (cmd_list*) malloc(sizeof(cmd_list));
  com->command = command;
  com->next=next;
  return com;
}

cmd* ast_if(Expr* cond, cmd_list* commands){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_IF;
  com -> comm.iff.cond = cond;
  com -> comm.iff.body = commands;
  return com;
}

cmd* ast_elseif(Expr* cond, cmd_list* commands){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_ELSEIF;
  com -> comm.elseif.cond = cond;
  com -> comm.elseif.body = commands;
  return com;
}

cmd* ast_else(cmd_list* commands){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_ELSE;
  com -> comm.elsee.body = commands;
  return com;
}

cmd* ast_while(Expr* cond, cmd_list* commands){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_WHILE;
  com -> comm.whilee.cond = cond;
  com -> comm.whilee.body = commands;
  return com;
}

cmd* ast_for(Expr* cond, Expr* size, cmd_list* commands){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_FOR;
  com -> comm.forr.cond = cond;
  com -> comm.forr.size = size;
  com -> comm.forr.body = commands;
  return com;
}

cmd* ast_in(char* value){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_IN;
  com -> comm.in.value = value;
  return com;
}

cmd* ast_out(Expr* expr){
  cmd* com = (cmd*) malloc(sizeof(cmd));
  com -> kind = C_OUT;
  com -> comm.out.expr = expr;
  return com;
}

