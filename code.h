typedef enum{
  I_PLUS, I_MINUS, I_DIV, I_TIMES, I_EQ, I_NOTEQ, I_GT, I_LT, I_GTEQ, I_LTEQ, I_ASSIGN, I_IF, I_GOTO, I_LABEL, I_INPUT, I_OUTPUT //devem ser diferentes dos ja definidos
}OpKind;

typedef enum{ 
  I_INT, I_VAR
}AddrKind;

struct Addr{
  AddrKind* kind;
  union{
    int val;
    char* var;
  }content;
};

struct Instruction{
  OpKind op;
  struct Addr *addr1, *addr2, *addr3;
};

struct List{
  struct Instruction* inst;
  struct List* next;
};

struct PairExpr{ 
  struct Addr* first;
  struct List* second; //Lista ligada de instrucoes de codigo intermedio
};

typedef struct List InstrList;
typedef struct Instruction Instr;
typedef struct Addr Address;
typedef struct PairExpr Pair;

Instr* mkInstr(OpKind op, Address* addr1, Address* addr2, Address* addr3);
InstrList* mkList(Instr* head, InstrList* tail);
InstrList* append(InstrList* l1,InstrList* l2);
Pair* compileExpr(Expr* e);
InstrList* compileCmd(cmd* c);
Address* mkIVar(char* name);
Address* mkIInt(int val);
  
