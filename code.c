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


