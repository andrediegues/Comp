#include <stdio.h>
#include "parser.h"

void print_tree(cmd_list* l){	
  if( l!=NULL){
    print_command(l->command);
    if(l->next !=NULL)
      print_tree(l->next);
    else return;
  }
  else{
    printf("NULL");
    return;
  }
}

  

void print_expr(Expr* expr){
  printf("Expressao(");
  switch(expr->kind){
		
  case E_INTEGER: printf("Int(%d ", expr->attr.value); break;
		
  case E_OPERATION:print_expr(expr->attr.op.left); printf("( ");
    switch(expr->attr.op.operator){
    case PLUS: printf("PLUS ) ");
      break;
    case MINUS: printf("MINUS ) ");
      break;
    case TIMES: printf("TIMES )");
      break;
    case DIV: printf("DIV )" );
      break;
    case MOD: printf("MOD )");
      break;
    case GT: printf(">) ");
      break;
    case GTEQ: printf(">=) ");
      break;
    case LT: printf("<) ");
      break;
    case LTEQ: printf("<=) ");
      break;
    case NOTEQ: printf("~=) ");
      break;
    case EQ: printf("==) ");
      break;
    default: break;
    }  
    print_expr(expr->attr.op.right);
    break;
  default: printf("Var( %s ) ",expr->attr.name);
  }
  printf(")");
}



void print_command(cmd* c){
	
  switch(c->kind){
  case IF:printf("IF ( ");
    print_expr(c->comm.iff.cond );
    print_tree(c->comm.iff.body); printf( " )");
    break;
    
  case ELSEIF: printf("ELSEIF ( ");
    print_expr(c->comm.elseif.cond);
    print_tree(c->comm.elseif.body); printf(" )");
    break;

  case ELSE: printf("ELSE ( ");
    print_tree(c->comm.elsee.body); printf(" ) ");
    break;

  case WHILE: printf("WHILE ( ");
    print_expr(c->comm.whilee.cond);
    print_tree(c->comm.whilee.body); printf( ") ");
    printf( ")");
    break;

  case FOR: printf("FOR ( ");
    print_expr(c->comm.forr.cond);
    print_expr(c->comm.forr.size);
    print_tree(c->comm.forr.body);
    printf(") ");
    break;

  case OUT: printf("OUTPUT ( ");
    print_expr(c->comm.out.expr);
    printf(") ");
    break;

  case IN: printf("INPUT ( Var(%s) ", c->comm.in.value);
    printf(") ");
    break;
    
  default: printf("Assignment( Var(%s) ", c->comm.assign.var);
    print_expr(c-> comm.assign.expr); printf(") ");
  }
}

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) {
    print_tree(root);
  }
  return 0;


}
