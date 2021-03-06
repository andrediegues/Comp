
// AST definitions
#ifndef __ast_h__
#define __ast_h__

typedef struct _Expr Expr; // Convenience typedef
typedef struct cmd_ cmd;
typedef struct Cmdlist cmd_list; 

// AST for expressions
struct _Expr {
  enum { 
    E_INTEGER,
    E_OPERATION,
    E_VAR
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

struct cmd_{
  enum{C_ASSIGN, C_IF, C_ELSEIF, C_ELSE, C_IN, C_OUT, C_WHILE, C_FOR} kind;
  union{
    struct{
      char* var;
      Expr* expr;
    }assign;
    struct{
      Expr* cond;
      cmd_list* body;
    }iff;
    struct{
      Expr* cond;
      cmd_list* body;
    }elseif;
    struct{
      cmd_list* body;
    }elsee;
    struct{
      Expr* cond;
      cmd_list* body;
    }whilee;
    struct{
      cmd* cond;
      Expr* size;
      cmd_list* body;
    }forr;
    struct{
      Expr* expr;
    }out;
    struct{
      char* value;
    }in;
  }comm;
};

struct Cmdlist{
  cmd* command;
  cmd_list* next;
};

  
// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_operation(int operator, Expr* left, Expr* right);
Expr* ast_variable(char* var);
cmd* ast_assign(char* var, Expr* expr);
cmd_list* ast_list(cmd* command, cmd_list* next);
cmd* ast_if(Expr* cond, cmd_list* commands);
cmd* ast_elseif(Expr* cond, cmd_list* commands);
cmd* ast_else(cmd_list* commands);
cmd* ast_while(Expr* cond, cmd_list* commands);
cmd* ast_for(cmd* cond, Expr* size, cmd_list* commands);
cmd* ast_in(char* value);
cmd* ast_out(Expr* expr);

#endif
