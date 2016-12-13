#include <stdio.h>
#include "parser.h"
#include "code.h"
#include "code.c"

/*void print_command(cmd* command);
void print_expr(Expr* expr);

void print_tree(cmd_list* l){	
  if( l!=NULL){
    print_command(l->command);
    if(l->next !=NULL)
      print_tree(l->next);
    else{ 
      printf("\n"); 
      return;
    }
  }
  else{
    printf("NULL");
    return;
  }
}

  

void print_expr(Expr* expr){

  printf("Expressao(");
  switch(expr->kind){
		
  case E_INTEGER: printf("Int(%d )", expr->attr.value); break;
		
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
	printf("\n");
  switch(c->kind){
  case C_IF:printf("IF ( ");
    print_expr(c->comm.iff.cond );
    print_tree(c->comm.iff.body); printf( " )");
    break;
    
  case C_ELSEIF: printf("ELSEIF ( ");
    print_expr(c->comm.elseif.cond);
    print_tree(c->comm.elseif.body); printf(" )");
    break;

  case C_ELSE: printf("ELSE ( ");
    print_tree(c->comm.elsee.body); printf(" ) ");
    break;

  case C_WHILE: printf("WHILE ( ");
    print_expr(c->comm.whilee.cond);
    print_tree(c->comm.whilee.body); printf( ") ");
    break;

  case C_FOR: printf("FOR ( ");
    print_command(c->comm.forr.cond);
    print_expr(c->comm.forr.size);
    print_tree(c->comm.forr.body);
    printf(") ");
    break;

  case C_OUT: printf("OUTPUT ( ");
    print_expr(c->comm.out.expr);
    printf(") ");
    break;

  case C_IN: printf("INPUT ( Var(%s) ", c->comm.in.value);
    printf(") ");
    break;
    
  default: printf("Assignment( Var(%s) ", c->comm.assign.var);
    print_expr(c-> comm.assign.expr); printf(") ");
  }
  }*/
int regist;
Pair compileExpr(Expr e);
InstrList compileCmd(Cmd c);
InstrList compileCmdList(CmdList l);

Pair compileExpr(Expr expr){
  Pair p = (Pair*) malloc(sizeof(Pair));
  switch(expr -> kind){
  case E_INT:
    p -> first = mkIInt(expr -> attr.val);
    p -> second = NULL;
    regist++;
    return p;
    break;
  case VAR:
    p -> first = mkIVar(expr -> attr.var);
    p -> second = NULL;
    return p;
    break;
  case E_OPERATION:
    Pair* p1 = compileExpr(expr->attr.op.left);
    Pair* p2 = compileExpr(expr->attr.op.right);
    Instr* t;

    switch(expr->attr.op.operator){
      case PLUS:
	t = mkInstr(I_PLUS, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case MINUS:
	t = mkInstr(I_MINUS, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case TIMES:
	t = mkInstr(I_TIMES, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case DIV:
	t = mkInstr(I_DIV, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case EQ:
	t = mkInstr(I_EQ, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case NOTEQ:
	t = mkInstr(I_NOTEQ, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case GT:
	t = mkInstr(I_GT, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case LT:
	t = mkInstr(I_LT, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case GTEQ:
	t = mkInstr(I_GTEQ, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
      case LTEQ:
	t = mkInstr(I_LTEQ, mkIVar(newVar(regist)),p1 -> first, p2 -> first);
	break;
    default: break;
    }
    p -> first = mkVaddr(newchar(regist));
    p -> second = append(append(p1->il,p2->il),mkList(t,NULL)); 
    regist++;
    return p;
    break;
  }
}

InstrList* compileCmd(Cmd* c){ 
  switch(cmd -> kind){
  case C_ASSIGN:
    p = compileExpr(cmd -> attr.assign.expr);
    l = append(p -> second, mkList(mkInstr(I_ASSIGN, mkIVar(cmd -> attr.assign.var), p -> first, NULL), NULL));
    return l;
    break;
    // case IF:
    // ...
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
    //    print_tree(root);
  }
  return 0;
}
