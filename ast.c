// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Var* ast_var(int type, char *name){
  Var* node = (Var*) malloc(sizeof(Var));
  node->name = name;
  node->type = type;
  return node;
}

VarList* ast_varlist(Var *var, VarList *next){
  VarList* node = (VarList*) malloc(sizeof(VarList));
  node->var = var;
  node->next = next;
  return node;
}

Expr* ast_expr_integer(int v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_expr_var(Var *var){
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VAR;
  node->attr.var = var;
  return node;
}

Expr* ast_expr_operation(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}


BoolExpr* ast_boolexpr_leaf(Expr* exp){
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_Bool;
  node->attr.value = exp;
  return node;
}

BoolExpr* ast_boolexpr(int operator, Expr* left, Expr* right){
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_BoolOp;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

BoolExpr* ast_boolexpr_complex(int operator, BoolExpr* left, BoolExpr* right){
  BoolExpr* node = (BoolExpr*) malloc(sizeof(BoolExpr));
  node->kind = E_Complex;
  node->attr.complex.operator = operator;
  node->attr.complex.left = left;
  node->attr.complex.right = right;
  return node;
}

Attrib* ast_attrib(Var *var, Expr *value){
  Attrib* node = (Attrib*) malloc(sizeof(Attrib));
  node->var = var;
  node->value = value;
  return node;
}

If* ast_if(BoolExpr* boolexpr, CmdList *cmdlist){
  If* node = (If*) malloc(sizeof(If));
  node->type = IFTYPE; //Acho que eh isso fon - APAGAR
  node->cmdlist = cmdlist;
  node->boolexpr = boolexpr;
  return node;
}

If* ast_if_else(BoolExpr* boolexpr, CmdList *cmdlist, CmdList *cmdlist_pos){
  If* node = (If*) malloc(sizeof(If));
  node->type = ELSETYPE; //Acho que eh isso fon - APAGAR
  node->cmdlist = cmdlist;
  node->cmdlist_pos = cmdlist_pos;
  node->boolexpr = boolexpr;
  return node;
}

While* ast_while(BoolExpr* boolexpr, CmdList *cmdlist){
  While* node = (While*) malloc(sizeof(While));
  node->boolexpr = boolexpr;
  node->cmdlist = cmdlist;
  return node;
}

For* ast_for(Attrib *init, BoolExpr *boolexpr, Attrib *inc, CmdList *cmdlist){
  For* node = (For*) malloc(sizeof(For));
  node->init = init;
  node->boolexpr = boolexpr;
  node->inc = inc;
  node->cmdlist = cmdlist;
  return node;
}

/*
For* ast_for(Var *var, Expr *value, BoolExpr *boolexpr, Var *incVar, Expr *incValue, CmdList *cmdlist){
  For* node = (For*) malloc(sizeof(For));
  Attrib* nodeAttrInit = (Attrib*) malloc(sizeof(Attrib));
  Attrib* nodeAttrInc = (Attrib*) malloc(sizeof(Attrib));
  //Tem que ser assim, precisa alocar espaço para as estruturas, se não fizer isso da segmentation fault.
  nodeAttrInit->var = var;
  nodeAttrInit->value = value;
  nodeAttrInc->var = incVar;
  nodeAttrInc->value = incValue;
  node->init = nodeAttrInit;
  node->boolexpr = boolexpr;
  node->inc = nodeAttrInc;
  node->cmdlist = cmdlist;
  return node;
}
*/

Printf* ast_printf(char* s, VarList *varlist){
  Printf* node = (Printf*) malloc(sizeof(Printf));
  node->s = s;
  node->varlist = varlist;
  return node;
}

Scanf* ast_scanf(char* s, VarList *varlist){
  Scanf* node = (Scanf*) malloc(sizeof(Scanf));
  node->s = s;
  node->varlist = varlist;
  return node;
}

Cmd* ast_cmd_attr(Attrib *cmdattrib){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_Attrib;
  node->attr.cmdattr = cmdattrib;
  return node;
}

Cmd* ast_cmd_if(If *cmdif){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_If;
  node->attr.cmdif = cmdif;
  return node;
}

Cmd* ast_cmd_for(For *cmdfor){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_For;
  node->attr.cmdfor = cmdfor;
  return node;
}

Cmd* ast_cmd_while(While *cmdwhile){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_While;
  node->attr.cmdwhile = cmdwhile;
  return node;
}

Cmd* ast_cmd_printf(Printf *cmdprintf){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_Printf;
  node->attr.cmdprintf = cmdprintf;
  return node;
}

Cmd* ast_cmd_scanf(Scanf *cmdscanf){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->type = E_Scanf;
  node->attr.cmdscanf = cmdscanf;
  return node;
}

CmdList* ast_cmdlist(Cmd *cmd, CmdList *next){
  CmdList* node = (CmdList*) malloc(sizeof(CmdList));
  node->value = cmd;
  node->next = next;
  return node;
}

Function* ast_function(char *name, CmdList *cmdlist){
  Function* node = (Function*) malloc(sizeof(Function));
  node->name = name;
  node->cmdlist = cmdlist;
  return node;
}