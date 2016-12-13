#include <string.h>
#include <stdlib.h>
#include "code.h"


Instr mkInstr(op, a1, a2, a3){
  Instr *instr = (Instr*) malloc(sizeof(Instr));
  instr -> op = op;
  instr -> addr1 = a1;
  instr -> addr2 = a2;
  instr -> addr3 = a3;
  return instr;
}

Address mkIInt(int i){
  Address *address = (Address*) malloc(sizeof(Address));
  address -> kind = I_Int;
  address -> content.val = i;
  return address;
}

Address mkIVar(char* x){
  Address *address = (Address*) malloc(sizeof(Address));
  address -> kind = I_VAR;
  address -> content.var = x;
  return address;
}

mkIEmpty(){

}

InstrList* mkList(Instr* head, InstrList* tail){
  InstrList* newp;
  newp = (InstrList*) malloc(sizeof(InstrList));
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
  for(p = l1; tail(p) != NULL; p = tail(p))
    tail(p) = l2;
  return l1;
}

Instr* head(InstrList* l){
  if(l != NULL)
    return (l -> inst);
}

InstrList* tail(InstrList* l){
  if(l != NULL)
    return (l -> next);
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



int regist;
int labels;
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
    p -> first = mkVaddr(newchar(regist));
    p -> second = append(append(p1->il,p2->il), mkList(t,NULL)); 
    regist++;
    return p;
    break;
  }
}

InstrList* compileCmd(Cmd* c){
  Pair* p = (Pair*) malloc(sizeof(Pair));
  InstrList* l = (InstrList*) malloc(sizeof(InstrList));
  InstrList* aux = (InstrList*) malloc(sizeof(InstrList));
  char *label1, *label2;
  label1 = label2 = NULL;
  switch(cmd -> kind){
  case C_ASSIGN:
    p = compileExpr(cmd -> attr.assign.expr);
    l = append(p -> second, mkList(mkInstr(I_ASSIGN, mkIVar(cmd -> attr.assign.var), p -> first, NULL), NULL));
    return l;
    break;
  case C_IF:
    labels++;
    label1 = newLabel(labels);
    labels++;
    label2 = newLabel(labels);

    p = compileExpr(cmd -> attr.iff.expr);
    l = append(p -> second, mkList(mkInstr(I_IF, mkIVar(label1), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label2), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL));

    aux = cmd -> attr.iff.next;
    while(aux != NULL){
      t = compileCmd(aux -> cmd);
      aux = aux -> next;
      l = append(l, t);
    }
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label2), NULL, NULL), NULL));
    return l;
    break;

    /* case C_ELSEIF:
    labels++;
    label1 = newLabel(labels); 
    labels++;
    label2 = newLabel(labels);
    char * E1 = newElse(elses);
    elses++;
    p = CompileExpression(command->attr.ifs.expr); 
    l = append(p -> second, mkList(mkInstr(I_IF,p -> first, mkIVar(label1), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO,mkIVar(label2), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL));
    aux = cmd -> attr.ifs.next; 
    while(aux != NULL){
      t = compileCmd(aux -> command);
      aux = aux -> next;
      l = append(l,t);
    }
    l = append(l, mkList(mkInstr(I_GOTO, mkVaddr(E1), NULL, NULL), NULL));
    l = append(l, mkList(mkIstr(I_LABEL, mkVaddr(label2), NULL, NULL), NULL)); 
    t = CompileIfs(command->attr.ifs.nextcondition,E1);
    l = append(l,t); 
    return l;
    break;*/

  case C_WHILE:
    p = compileExpr(command -> attr.whiles.expr);
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
    aux = cmd->attr.whilee.next;
    while(aux != NULL){
      t = compileCmd(aux -> command);
      aux = aux -> next;
      l = append(l,t);
    } 
    l = append(l, mkList(mkInstr(I_GOTO, mkVaddr(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkVaddr(label2), NULL, NULL), NULL)); 
    return l;
    break;
  case C_FOR:
    p = compileExpr(command->attr.forr.expr); 
    labels++;
    label1 = newLabel(labels);
    labels++;
    label2 = newLabel(labels);
    labels++;
    label3 = newLabel(labels);
    size = mkInstr(I_ASSIGN, mkIVar(cmd -> attr.forr.var), mkIInt(cmd -> attr.forr.size), NULL);
    Address newAddress = mkIVar(newVar(regist));
    regist++;
    
    l = append(mkList(size, p -> second), mkList(mkInstr(I_LABEL, mkIVar(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LESS, newAddress, mkIVar(command->attr.forr.var), p -> first), NULL));
    l = append(l, mkList(mkInstr(I_IF, newAddress, mkIVar(label3), NULL), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkIVar(label2), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkVaddr(label3), NULL, NULL), NULL));
    aux = cmd -> attr.forr.next;
    while(aux != NULL){
      t = compileCmd(aux -> command);
      aux = aux -> next;
      l = append(l,t);
    }
    l = append(l, mkList(mkInstr(I_PLUS, mkIVar(cmd -> attr.forr.var), mkIVar(cmd -> attr.forr.var), mkIInt(1)), NULL));
    l = append(l, mkList(mkInstr(I_GOTO, mkVaddr(label1), NULL, NULL), NULL));
    l = append(l, mkList(mkInstr(I_LABEL, mkVaddr(label2), NULL, NULL), NULL)); 
    return l;
    break;
    
  case C_OUT:
    p = compileExpr(command -> attr.out.expr);
    l = append(p -> second, mkList(mkInstr(I_OUTPUT, p -> first, NULL, NULL), NULL));
    return l;
    break;

  case C_IN: 
    l = mkList(mkIstr(I_INPUT, mkIVar(cmd -> attr.in.name), NULL, NULL), NULL);
    return l;
    break;

  default:
    break;
  }
}

/*IList* CompileIfs(Ifs* command,char * E){
  IList* l=NULL;
  IList* t=NULL;
  Pair* p =NULL;
  char* L0=NULL;
  char* L1=NULL;
  char* L2=NULL;
  Istr* i = NULL;
  List* help = NULL;

  switch(command->kind){

    case IF_ELSEIF:// DONE
      //struct {struct Expression* expr; struct CommandList* next; struct IfList* nextcondition;} elseifs;

      lab++; L0 = newLabel(lab); //Label for Begin of If
      lab++; L1 = newLabel(lab);  //Label for End of If

      p = CompileExpression(command->attr.elseifs.expr); 

      l = joinLists(p->il,mkList(mkIstr(I_IF,p->name,mkVaddr(L0),NULL),NULL)); //evaluate the expression -> mkIntr esta mal
      l = joinLists(l,mkList(mkIstr(I_GOTO,mkVaddr(L1),NULL,NULL),NULL));

      // IF expression true down

      l = joinLists(l,mkList(mkIstr(I_LABEL,mkVaddr(L0),NULL,NULL),NULL)); // if expression true come here
      help=command->attr.elseifs.next; //COMLIST to iterate the commands inside the if
      while(help!=NULL){ t=CompileCom(help->command); help=help->next; l = joinLists(l,t);} //join all comands INSIDE the if to Ilist
      l = joinLists(l,mkList(mkIstr(I_GOTO,mkVaddr(E),NULL,NULL),NULL)); //end of if go after all ifs

      // IF Expression TRUE UP

      l = joinLists(l,mkList(mkIstr(I_LABEL,mkVaddr(L1),NULL,NULL),NULL)); //OUT OF IF
      t = CompileIfs(command->attr.elseifs.nextcondition,E); //Print next condition 
      l = joinLists(l,t); //join the Lists

      return l;


    case IF_ELSE: //DONE
      //struct {struct CommandList* next;} elses;

      help=command->attr.elses.next; //COMLIST 
      while(help!=NULL){ t = CompileCom(help->command); help=help->next;l=joinLists(l,t);}

      l = joinLists(l,mkList(mkIstr(I_LABEL,mkVaddr(E),NULL,NULL),NULL)); //end of if go after all ifs

      return l;


    case IF_ELIF: // IF EVALUATE ERROR
      //struct {struct Expression* expr; struct CommandList* next;} elif;

      lab++; L0 = newLabel(lab); //Label for Begin of If

      p = CompileExpression(command->attr.elif.expr); 

      l = joinLists(p->il,mkList(mkIstr(I_IF,p->name,mkVaddr(L0),NULL),NULL)); //evaluate the expression 
      l = joinLists(l,mkList(mkIstr(I_GOTO,mkVaddr(E),NULL,NULL),NULL));
      l = joinLists(l,mkList(mkIstr(I_LABEL,mkVaddr(L0),NULL,NULL),NULL)); // if expression true come here

      help=command->attr.elif.next; //COMLIST for iterate ...
\      while(help!=NULL){ t = CompileCom(help->command); help=help->next;joinLists(l,t);}

      l = joinLists(l,mkList(mkIstr(I_LABEL,mkVaddr(E),NULL,NULL),NULL)); //end of if go after all ifs

      return l;

  }
}
*/
void printAssembly(FILE* file, InstrList* il){

  if(il==NULL)
    return;
  Instr* instr = il -> instr;

  char *s1, *s2, *s3;
  s1 = (char*) malloc(20*sizeof(char));
  s2 = (char*) malloc(20*sizeof(char));
  s3 = (char*) malloc(20*sizeof(char));

  if(instr -> addr1 != NULL){ 
    if(instr -> addr1 -> op == Int)
      sprintf(s1, "%d", instr -> addr1 -> content.val); 
    else{
      strcat(s1, strdup(instr -> addr1 -> content.name)); }  
  }

  if(instr -> addr2 != NULL){ 
    if(instr -> addr2 -> op == Int){
      sprintf(s2, "%d", instr -> addr2 -> content.val);
    } 
    else{
      strcat(s2, strdup(instr -> addr2 -> content.name));
    } 
  }

  if(instr -> addr3 != NULL){
    if(instr -> addr3 -> op == Int){ 
      sprintf(s3, "%d", instr -> addr3 -> content.val);
    } 
    else{
      strcat(s3, strdup(instr -> addr3 -> content.name));
    } 
  }


  switch(instr -> op){

    case I_PLUS:
      fprintf(file, "add %s, %s, %s \n", instr -> addr1 -> content.name, s2, s3);
      break;

    case I_MINUS:
      fprintf(file, "sub %s, %s, %s \n", s1, s2, s3);
      break;

    case I_DIV:
      fprintf(file, "div %s, %s \n", s2, s3);
      fprintf(file, "add %s $LO $0 \n", s1);
      break;

    case I_MUL:
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

  printAssembly(file, il -> next); // go to next Instruction
}
