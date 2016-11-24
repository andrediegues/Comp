#include "code.h"

mkInstr(op, a1, a2, a3){
	...
}

mkIInt(int i){
	...
}

mkIVar(char* x){
	...
}

mkIEmpty(){

}

List mkList(Inst head, List tail){
	List* newp;
	newp = (List*) malloc(sizeof(List));
	newp -> inst = head;
	newp -> next = tail;
	return newp; 
}

List append(List* l1, List* l2){
	List* p;
	if(l1 == NULL)
		return l2;
	if(l2 == NULL)
		return l1;
	for(p = l1; tail(p) != NULL; p = tail(p))
		tail(p) = l2;
	return l1;
}



Inst head(List l){
	if(l != NULL)
		return (l -> inst);
}

List tail(List l){
	if(l != NULL)
		return (l -> next;)
}

Pair compileExpr(Expr e);
InstrList compileCmd(Cmd c);
InstrList compileCmdList(CmdList l);

InstrList compileCmd(Cmd c){
	...
	switch(cmd... -> kind){
		case ASSIGN:
			p = compileExpr(cmd... -> expr);
			l = append(p -> code/*snd(p)*/, mkList(mkInstr(ASSIGN, cmd -> var/*var(cmd)*/, p -> var/*fst(p)*/, NULL)));
			return l;
		case IF:
		...
	}
}
