
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum { 
    E_INTEGER,
    E_OPERATION
  } kind;
  union {
    int value; // for integer values
    struct { 
      int operator; // PLUS, MINUS, etc 
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

/*struct Exprlist{
  _Expr *exp;
  Exprlist *next;
  };*/

typedef struct _Expr Expr; // Convenience typedef
typedef struct cmd_ *cmd;
/*struct cmd_{
  enum{ASSIGN, IF, ELSEIF, END, WHILE, FOR} kind;
  union{
    struct{
      char var[200];
      Expr expr;
    }assign;
    struct{
      Expr comd;
      cmd_list body;
    }iff;
  }
  }*/
//typedef enum {PLUS,MINUS,TIMES,DIV} operator;

// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_operation(int operator, Expr* left, Expr* right);

#endif
