#include <stdio.h>
#include "parser.h"
#include "code.h"
#include <stdlib.h>
#include <string.h>


int regist;
int labels;

Instr* mkInstr(OpKind op, Address* a1, Address* a2, Address* a3){
  Instr* instr = (Instr*) malloc(sizeof(Instr));
  instr -> op = op;
  instr -> addr1 = a1;
  instr -> addr2 = a2;
  instr -> addr3 = a3;
  return instr;
}

Address* mkIInt(int i){
  Address *address = malloc(sizeof(Address));
  address -> kind = I_INT;
  address -> content.val = i;
  return address;
}

Address* mkIVar(char* x){
  Address* address = malloc(sizeof(Address));
  address -> kind = I_VAR;
  address -> content.var = malloc(20*sizeof(char));
  strcpy(address -> content.var, x);
  return address;
}

InstrList* mkList(Instr* head, InstrList* tail){
  InstrList* newp = (InstrList*) malloc(sizeof(InstrList));
  newp -> inst = head;
  newp -> next = tail;
  return newp; 
}

InstrList* append(InstrList* l1, InstrList* l2){
  InstrList* p;
  if(l1 == NULL)
    return l2;
  if(l2 == NULL)
    return l1;
  for(p = l1; p -> next != NULL; p = p -> next);
  p -> next = l2;
  return l1;
}

char* newVar(int n){
  char t1[20];
  char t2[20];
  strcpy(t1, "$t");
  sprintf(t2, "%d", n);
  char* aux = (char*) malloc(1 + strlen(t1) + strlen(t2));
  strcpy(aux, t1);
  strcat(aux, t2);
  return aux;
}

char* newLabel(int n){
  char t1[20];
  char t2[20];
  strcpy(t1, "L");
  sprintf(t2, "%d", n);
  char* aux = (char*) malloc(1 + strlen(t1) + strlen(t2));
  strcpy(aux, t1);
  strcat(aux, t2);
  return aux;
}



Pair* compileExpr(Expr* expr){
  Pair* p = (Pair*) malloc(sizeof(Pair));
  if(expr -> kind == E_INTEGER){
    p -> first = mkIInt(expr -> attr.value);
    p -> second = NULL;
    regist++;
    return p;
  }
  else if(expr -> kind == E_VAR){
    p -> first = mkIVar(expr -> attr.name);
    p -> second = NULL;
    return p;
  }
  else if(expr -> kind == E_OPERATION){
    Pair* p1 = compileExpr(expr -> attr.op.left);
    Pair* p2 = compileExpr(expr -> attr.op.right);
    Instr* t;

    switch(expr->attr.op.operator){
    case PLUS:
      t = mkInstr(I_PLUS, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case MINUS:
      t = mkInstr(I_MINUS, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case TIMES:
      t = mkInstr(I_TIMES, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case DIV:
      t = mkInstr(I_DIV, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case EQ:
      t = mkInstr(I_EQ, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case NOTEQ:
      t = mkInstr(I_NOTEQ, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case GT:
      t = mkInstr(I_GT, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case LT:
      t = mkInstr(I_LT, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case GTEQ:
      t = mkInstr(I_GTEQ, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    case LTEQ:
      t = mkInstr(I_LTEQ, mkIVar(newVar(regist)), p1 -> first, p2 -> first);
      break;
    default: break;
    }
    p -> first = mkIVar(newVar(regist));
    p -> second = append(append(p1 -> second, p2 -> second), mkList(t,NULL)); 
    regist++;
    return p;
  }
}

InstrList* compileCmd(cmd* cmd){
  
  Pair* p = NULL;
  InstrList* l = NULL;
  InstrList* t = NULL;
  InstrList* aux = NULL;
  Instr* size = NULL;
  char *label1, *label2, *label3;
  label1 = label2 = label3 = NULL;
  
  switch(cmd -> kind){
    
  case C_ASSIGN:
    p = compileExpr(cmd -> comm.assign.expr);
    l = append(p -> second, mkList(mkInstr(I_ASSIGN, mkIVar(cmd -> comm.assign.var), p -> first, NULL), NULL));
    return l;
    
  case C_IF:
    labels++;
    label1 = newLabel(labels);
    labels++;
    label2 = newLabel(labels);

    p = compileExpr(cmd -> comm.iff.cond);
    l = append(p -> second, mkList(mkInstr(I_IF, p -> first, mkIVar(label1), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label2), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL));

    aux = cmd -> comm.iff.body;
    while(aux != NULL){
      t = compileCmd(aux -> inst);
      l = append(l, t);
      aux = aux -> next;
    }
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label2), NULL, NULL), NULL));
    return l;

  case C_WHILE:
    p = compileExpr(cmd -> comm.whilee.cond);
    labels++;
    label1 = newLabel(labels);
    labels++;
    label2 = newLabel(labels);
    labels++;
    label3 = newLabel(labels);
      
    l = append(mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL), p -> second);
    l = append(l, mkList(mkInstr(I_IF, p -> first, mkIVar(label3), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label2), NULL, NULL), NULL)); 
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label3), NULL, NULL), NULL));
    aux = cmd -> comm.whilee.body;
    while(aux != NULL){
      t = compileCmd(aux -> inst);
      l = append(l,t);
      aux = aux -> next;
    } 
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label2), NULL, NULL), NULL)); 
    return l;

  case C_FOR:
    p = compileExpr(cmd -> comm.forr.cond); 
    labels++;
    label1 = newLabel(labels);
    labels++;
    label2 = newLabel(labels);
    labels++;
    label3 = newLabel(labels);
    size = mkInstr(I_ASSIGN, mkIVar(cmd -> comm.forr.cond), mkIInt(cmd -> comm.forr.size), NULL);
    Address* newAddress = mkIVar(newVar(regist));
    regist++;
    
    l = append(mkList(size, p -> second), mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LT, newAddress, mkIVar(cmd -> comm.forr.cond), p -> first), NULL));
    l = append(l, mkList(mkInstr(I_IF, newAddress, mkIVar(label3), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label2), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label3), NULL, NULL), NULL));
    aux = cmd -> comm.forr.body;
    while(aux != NULL){
      t = compileCmd(aux -> inst);
      l = append(l,t);
      aux = aux -> next;
    }
    l = append(l, mkList(mkInstr(I_PLUS, mkIVar(cmd -> comm.forr.cond), mkIVar(cmd -> comm.forr.cond), mkIInt(1)), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label2), NULL, NULL), NULL)); 
    return l;
    
  case C_OUT:
    p = compileExpr(cmd -> comm.out.expr);
    l = append(p -> second, mkList(mkInstr(I_OUTPUT, p -> first, NULL, NULL), NULL));
    return l;

  case C_IN: 
    l = mkList(mkInstr(I_INPUT, mkIVar(cmd -> comm.in.value), NULL, NULL), NULL);
    return l;

  default:
    break;
  }
}

void printAssembly(FILE* file, InstrList* l){

  if(l == NULL)
    return;
  Instr* instr = l -> inst;

  char *s1, *s2, *s3;
  s1 = (char*) malloc(20*sizeof(char));
  s2 = (char*) malloc(20*sizeof(char));
  s3 = (char*) malloc(20*sizeof(char));

  if(instr -> addr1 != NULL){ 
    if(instr -> addr1 -> kind == I_INT)
      sprintf(s1, "%d", instr -> addr1 -> content.val); 
    else{
      strcat(s1, strdup(instr -> addr1 -> content.var)); }  
  }

  if(instr -> addr2 != NULL){ 
    if(instr -> addr2 -> kind == I_INT){
      sprintf(s2, "%d", instr -> addr2 -> content.val);
    } 
    else{
      strcat(s2, strdup(instr -> addr2 -> content.var));
    } 
  }

  if(instr -> addr3 != NULL){
    if(instr -> addr3 -> kind == I_INT){ 
      sprintf(s3, "%d", instr -> addr3 -> content.val);
    } 
    else{
      strcat(s3, strdup(instr -> addr3 -> content.var));
    } 
  }


  switch(instr -> op){

    case I_PLUS:
      fprintf(file, "add %s, %s, %s \n", instr -> addr1 -> content.var, s2, s3);
      break;

    case I_MINUS:
      fprintf(file, "sub %s, %s, %s \n", s1, s2, s3);
      break;

    case I_DIV:
      fprintf(file, "div %s, %s \n", s2, s3);
      fprintf(file, "add %s $LO $0 \n", s1);
      break;

    case I_TIMES:
      fprintf(file, "mul %s, %s, %s\n", s1, s2, s3);
      break;

    case I_EQ:
      fprintf(file, "seq %s, %s, %s\n", s1, s2, s3); //set equal
      break;

    case I_NOTEQ:
      fprintf(file, "sne %s, %s, %s\n", s1, s2, s3); // set not equal
      break;

    case I_GT:
      fprintf(file, "sgt %s, %s, %s\n", s1, s2, s3); // set greater 
      break;

    case I_LT:
      fprintf(file, "slt %s, %s, %s\n", s1, s2, s3); // set less 
      break;

    case I_GTEQ:
      fprintf(file, "sge %s, %s, %s\n", s1, s2, s3); //set greater or equal
      break;

    case I_LTEQ:
      fprintf(file, "sle %s, %s, %s\n", s1, s2, s3); // set ledd or equal 
      break;
      
    case I_ASSIGN:
      fprintf(file, "add %s, %s, $0\n", s1, s2); 
      break;
      
    case I_IF:
      fprintf(file, "bne %s, $0, %s\n", s1, s2);  //branch not equal
      break;

    case I_LABEL:
      fprintf(file, "%s:\n", s1); 
      break;

    case I_GOTO:
      fprintf(file, "j %s\n", s1); //JUMP 
      break;

    case I_INPUT:
      fprintf(file, "addi $v0 , $0 , 5\n"); 
      fprintf(file, "syscall\n");   //get input
      fprintf(file, "move %s, $v0 \n", s1);
      break;

    case I_OUTPUT:
      fprintf(file, "addi $v0 , $0 , 4\n");
      fprintf(file, "la $a0 , %s\n", s1);
      fprintf(file, "syscall\n");     //printing s1
      break;

    default: 
      break;
  }

  printAssembly(file, l -> next); // go to next Instruction
}


int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }
  FILE *file = fopen(argv[1], "w+");
  if (yyparse() == 0) {
    while(root != NULL){
      InstrList* il = compileCmd(root -> command);
      printAssembly(file, il);
      regist = 0;
      root = root -> next;
    }
  }
  fclose(file);
  return 0;
}
