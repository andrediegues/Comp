
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum { 
    E_INTEGER,
    E_OPERATION,
  } kind;
  union {
    int value; // for integer values
    char* name;
    struct { 
      int operator; // PLUS, MINUS, etc 
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

struct Cmdlist{
  cmd* command;
  Cmdlist* next;
};

typedef struct _Expr Expr; // Convenience typedef
typedef struct cmd_ cmd;
typedef struct Cmdlist cmd_list; 
struct cmd_{
  enum{ASSIGN, IF, ELSEIF, ELSE, IN, OUT, WHILE, FOR} kind;
  union{
    struct{
      char var[200];
      Expr expr;
    }assign;
    struct{
      Expr cond;
      cmd_list body;
    }iff;
    //to do: elseif, else, in, out, while and for
  }
}
  
// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_operation(int operator, Expr* left, Expr* right);
Expr* ast_variable(char* var);
//to do: declare functions of commands and list to complete in the ast.c file

#endif
