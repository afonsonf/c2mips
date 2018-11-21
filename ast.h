
// AST definitions
#ifndef __ast_h__
#define __ast_h__

struct _Var{
  int type;
  char *name;
};

struct _VarList{
  struct _Var *var;
  struct _VarList *next;
};

struct _Expr {
  enum {
    E_INTEGER,
    E_VAR,
    E_OPERATION
  } kind;
  union {
    int value; // for integer values
    struct {
      int operator; // PLUS, MINUS, etc
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
    struct _Var* var;
  } attr;
};


struct _BoolExpr{
  enum {
    E_Bool,
    E_BoolOp,
    E_Complex
  } kind;
  union {
    struct _Expr* value; // expr
    struct {
      int operator; // ==,<,>,<=,>=,!=
      struct _Expr* left;
      struct _Expr* right;
    } op;
    struct {
      int operator; // &&, ||
      struct _BoolExpr* left;
      struct _BoolExpr* right;
    } complex;
  } attr;
};

struct _Attrib{
  struct _Var *var;
  struct _Expr *value;
};

struct _If{
  enum {IFTYPE, ELSETYPE} type;
  struct _BoolExpr *boolexpr;
  struct _CmdList *cmdlist;
  struct _CmdList *cmdlist_pos;
};

struct _While{
  struct _BoolExpr *boolexpr;
  struct _CmdList *cmdlist;
};

struct _For{
  struct _Attrib *init;
  struct _BoolExpr *boolexpr;
  struct _Attrib *inc;
  struct _CmdList *cmdlist;
};

struct _Printf{
  char *s;
  struct _VarList *varlist;
};

struct _Scanf{
  char *s;
  struct _VarList *varlist;
};

struct _Cmd {
  enum {
    E_Attrib,
    E_If,
    E_For,
    E_While,
    E_Printf,
    E_Scanf
  }type;
  union {
    struct _Attrib *cmdattr;
    struct _If *cmdif;
    struct _For *cmdfor;
    struct _While *cmdwhile;
    struct _Printf *cmdprintf;
    struct _Scanf *cmdscanf;
  }attr;
};

struct _CmdList{
  struct _Cmd *value;
  struct _CmdList *next;
};

struct _Function{
  char* name;
  struct _CmdList *cmdlist;
};

typedef struct _Var Var;
typedef struct _VarList VarList;
typedef struct _Expr Expr; // Convenience typedef
typedef struct _BoolExpr BoolExpr; // Convenience typedef

typedef struct _Attrib Attrib;
typedef struct _If If;
typedef struct _For For;
typedef struct _While While;
typedef struct _Printf Printf;
typedef struct _Scanf Scanf;

typedef struct _Cmd Cmd;
typedef struct _CmdList CmdList;

typedef struct _Function Function;

Var* ast_var(int type, char *name);
VarList* ast_varlist(Var *var, VarList *next);

Expr* ast_expr_integer(int v);
Expr* ast_expr_var(Var *var);
Expr* ast_expr_operation(int operator, Expr* left, Expr* right);

BoolExpr* ast_boolexpr_leaf(Expr* exp);
BoolExpr* ast_boolexpr(int operator, Expr* left, Expr* right);
BoolExpr* ast_boolexpr_complex(int operator, BoolExpr* left, BoolExpr* right);

Attrib* ast_attrib(Var *var, Expr *value);
If* ast_if(BoolExpr* boolexpr, CmdList *cmdlist);
If* ast_if_else(BoolExpr* boolexpr, CmdList *cmdlist, CmdList *cmdlist_pos);
While* ast_while(BoolExpr* boolexpr, CmdList *cmdlist);
For* ast_for(Attrib *init, BoolExpr *boolexpr, Attrib *inc, CmdList *cmdlist);
//For* ast_for(Var *var, Expr *value, BoolExpr *boolexpr, Var *incVar, Expr *incValue, CmdList *cmdlist);
Printf* ast_printf(char* s, VarList *varlist);
Scanf* ast_scanf(char* s, VarList *varlist);

Cmd* ast_cmd_attr(Attrib *cmdattrib);
Cmd* ast_cmd_if(If *cmdif);
Cmd* ast_cmd_for(For *cmdfor);
Cmd* ast_cmd_while(While *cmdwhile);
Cmd* ast_cmd_printf(Printf *cmdprintf);
Cmd* ast_cmd_scanf(Scanf *cmdscanf);

CmdList* ast_cmdlist(Cmd *cmd, CmdList *next);

Function* ast_function(char *name, CmdList *cmdlist);

#endif
