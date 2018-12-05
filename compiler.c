#include <stdio.h>
#include "parser.h"
#include "stack.h"

InstrList *expr2instr(Expr *expr);
InstrList *boolexpr2instr(BoolExpr* boolexpr);

InstrList *atrib2instr(Attrib *atrib);
InstrList *if2instr(If* ifcmd);

InstrList *cmd2instr(Cmd *cmd);
InstrList *cmdlist2instr(CmdList *cmdlist);

int currLabel;

InstrList *expr2instr(Expr *expr)
{
  if (expr->kind == E_INTEGER)
  {
    Instr *no = mk_instr_ldc(expr->attr.value);
    return mk_instrlist(no, NULL);
  }
  else if (expr->kind == E_VAR)
  {
    Instr *no = mk_instr_lod(expr->attr.var->name);
    return mk_instrlist(no, NULL);
  }
  else if (expr->kind == E_OPERATION)
  {
    Instr *no;
    switch (expr->attr.op.operator)
    {
    case PLUS:
      no = mk_instr(E_ADI);
      break;
    case MINUS:
      no = mk_instr(E_SBI);
      break;
    case MOD:
      no = mk_instr(E_MOD);
      break;
    case MULT:
      no = mk_instr(E_MPI);
      break;
    case DIV:
      no = mk_instr(E_DVI);
      break;
    }

    InstrList *instrlist = expr2instr(expr->attr.op.left);
    instrlist_append(instrlist, expr2instr(expr->attr.op.right));
    instrlist_append_instr(instrlist, no);

    return instrlist;
  }
}

InstrList *boolexpr2instr(BoolExpr* boolexpr){
  if(boolexpr->kind == E_Bool){
    InstrList *node;
    node = expr2instr(boolexpr->attr.value);
    Instr* no = mk_instr_ldc(0);
    instrlist_append_instr(node,no);
    no = mk_instr(E_NEQ);
    instrlist_append_instr(node,no);
    return node;
  }else if(boolexpr->kind == E_BoolOp){
    Instr* opr;
    switch (boolexpr->attr.op.operator) {
      case EQUALS:
        opr = mk_instr(E_EQU);
        break;
      case DIFF:
        opr = mk_instr(E_NEQ);
        break;
      case LESS:
        opr = mk_instr(E_LES);
        break;
      case GREAT:
        opr = mk_instr(E_GES);
        break;
      case LESSEQUAL:
        opr = mk_instr(E_LEQ);
        break;
      case GREATEQUAL:
        opr = mk_instr(E_GEQ);
        break;
    }
    InstrList* result = expr2instr(boolexpr->attr.op.left);
    instrlist_append(result, expr2instr(boolexpr->attr.op.right));
    instrlist_append_instr(result, opr);
    return result;
  }
}

InstrList *atrib2instr(Attrib *atrib)
{
  InstrList *decl = NULL;
  if (atrib->var->type == VARINT)
  {
    Instr *tmp = mk_instr_dcl_var(atrib->var->name);
    decl = mk_instrlist(tmp, NULL);
  }

  InstrList *result = NULL;
  Instr *no = mk_instr_lda(atrib->var->name);
  if (decl)
  {
    result = decl;
    instrlist_append_instr(result, no);
  }
  else
  {
    result = mk_instrlist(no, NULL);
  }

  if(!atrib->value) return result;

  InstrList *node = expr2instr(atrib->value);
  instrlist_append(result, node);

  no = mk_instr(E_STO);
  instrlist_append_instr(result, no);
  return result;
}

InstrList *if2instr(If* ifcmd){
  if(ifcmd->type == IFTYPE){
    int lbl_if_end = currLabel;currLabel++;

    InstrList *res = boolexpr2instr(ifcmd->boolexpr);
    Instr *no = mk_instr_fjp(lbl_if_end);
    instrlist_append_instr(res,no);

    InstrList *tmp = cmdlist2instr(ifcmd->cmdlist);
    no = mk_instr_lbl(lbl_if_end);
    instrlist_append_instr(tmp,no);
    instrlist_append(res,tmp);

    return res;
  }

  int lbl_else_start = currLabel;currLabel++;
  int lbl_else_end = currLabel;currLabel++;

  InstrList *res = boolexpr2instr(ifcmd->boolexpr);
  Instr *no = mk_instr_fjp(lbl_else_start);
  instrlist_append_instr(res,no);

  InstrList *tmp = cmdlist2instr(ifcmd->cmdlist);

  no = mk_instr_ujp(lbl_else_end);
  instrlist_append_instr(tmp,no);

  no = mk_instr_lbl(lbl_else_start);
  instrlist_append_instr(tmp,no);

  instrlist_append(res,tmp);

  tmp = cmdlist2instr(ifcmd->cmdlist_pos);

  no = mk_instr_lbl(lbl_else_end);
  instrlist_append_instr(tmp,no);

  instrlist_append(res,tmp);
  return res;

}

InstrList *while2instr(While *whilecmd){
  int while_start = currLabel;
  currLabel++;
  int while_end = currLabel;
  currLabel++;

  Instr *no = mk_instr_lbl(while_start);
  InstrList *result = mk_instrlist(no,NULL);
  InstrList *tmp = boolexpr2instr(whilecmd->boolexpr);
  instrlist_append(result,tmp);
  no = mk_instr_fjp(while_end);
  instrlist_append_instr(result,no);

  return result;
}

InstrList *cmd2instr(Cmd *cmd)
{
  switch (cmd->type)
  {
  case E_Attrib:
    return atrib2instr(cmd->attr.cmdattr);
  case E_If:
    return if2instr(cmd->attr.cmdif);
  case E_While:
  //return while2instr(cmd->attr.cmdwhile);
  case E_Printf:
  //return printf2instr(cmd->attr.cmdprintf);
  case E_Scanf:
    //return scanf2instr(cmd->attr.cmdscanf);
    return NULL;
  }
  return NULL;
}

InstrList *cmdlist2instr(CmdList *cmdlist)
{
  if (!cmdlist)
    return NULL;
  InstrList *next = cmdlist2instr(cmdlist->next);
  InstrList *node = cmd2instr(cmdlist->value);
  instrlist_append(node, next);
  return node;
}

InstrList *function2instr(Function *fun){
  Instr *instr = mk_instr_lbl(currLabel++);
  InstrList *next = cmdlist2instr(fun->cmdlist);
  InstrList *res = mk_instrlist(instr,next);

  return res;
}

/*
void printMips(InstrList *instrlist)
{
  if (instrlist)
  {
    switch (instrlist->node->type)
    {
    case E_LDC:
      printf("addi $t0 $0 %d\n", instrlist->node->attr.num);
      printf("sw $t0, 4($sp)\n");
      printf("addi $sp -4\n");
      break;
    case E_ADI:
      printf("add $t0 0($sp) 4($sp)\n");
      printf("addi $sp 8\n");
      printf("sw $t0, 4($sp)\n");
      printf("addi $sp -4\n");
      break;
    case E_MPI:
      printf("mult 0($sp) 4($sp)\n");
      printf("addi $sp 8\n");
      printf("sw $hi, 4($sp)\n");
      printf("addi $sp -4\n");
      break;
    case E_SBI:
      printf("subb $t0 0($sp) 4($sp)\n");
      printf("addi $sp 8\n");
      printf("sw $t0, 4($sp)\n");
      printf("addi $sp -4\n");
      break;
    }
  }
  return;
}*/

int main(int argc, char **argv)
{
  --argc;
  ++argv;
  if (argc != 0)
  {
    yyin = fopen(*argv, "r");
    if (!yyin)
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0)
  {
    currLabel = 0;
    InstrList *res = function2instr(root);

    print_InstrList(res);
  }
  return 0;
}
