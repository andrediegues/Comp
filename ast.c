// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr* ast_integer(int v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_operation
(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}
//to-do: all functions constructors in ast.h
/*Exprlist Ast_Exprlist(_Expr *e, Exprlist *rest){
	 Exprlist *elem = (Exprlist*) malloc(sizeof(Exprlist));
	 elem -> exp = e;
	 elem -> next = rest;
	 return elem;
	 }*/
