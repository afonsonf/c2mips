#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
  return NULL;
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
  return NULL;
}

InstrList *atrib2instr(Attrib *atrib)
{
  InstrList *result = NULL;

  if (atrib->var->type == VARINT)
  {
    Instr *tmp = mk_instr_dcl_var(atrib->var->name);
    result = mk_instrlist(tmp,NULL);
  }
  if(!atrib->value)
    return result;

  InstrList *node = expr2instr(atrib->value);

  if(result == NULL) result = node;
  else instrlist_append(result, node);

  Instr *no = mk_instr_sto(atrib->var->name);
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

  tmp = cmdlist2instr(whilecmd->cmdlist);
  instrlist_append(result,tmp);
  no = mk_instr_ujp(while_start);
  instrlist_append_instr(result,no);

  no = mk_instr_lbl(while_end);
  instrlist_append_instr(result,no);


  return result;
}

InstrList *scanf2instr(Scanf *cmd){
  if(!cmd->varlist) return NULL;

  VarList* varlist = cmd->varlist;

  Instr* no = mk_instr_read(varlist->var->name);
  InstrList* res = mk_instrlist(no,NULL);

  varlist = varlist->next;
  while(varlist){
    no = mk_instr_read(varlist->var->name);
    instrlist_append_instr(res,no);
    varlist = varlist->next;
  }

  return res;
}

InstrList *printf2instr(Printf *cmd){
  if(!cmd->varlist) return NULL;

  VarList* varlist = cmd->varlist;

  Instr* no = mk_instr_wrt(varlist->var->name);
  InstrList* res = mk_instrlist(no,NULL);

  varlist = varlist->next;
  while(varlist){
    no = mk_instr_wrt(varlist->var->name);
    instrlist_append_instr(res,no);
    varlist = varlist->next;
  }

  return res;
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
    return while2instr(cmd->attr.cmdwhile);
  case E_Printf:
    return printf2instr(cmd->attr.cmdprintf);
  case E_Scanf:
    return scanf2instr(cmd->attr.cmdscanf);
  }
  return NULL;
}

InstrList *cmdlist2instr(CmdList *cmdlist)
{
  if (!cmdlist)
    return NULL;

  InstrList *next = cmdlist2instr(cmdlist->next);
  InstrList *node = cmd2instr(cmdlist->value);

  if(node) instrlist_append(node, next);
  else return next;

  return node;
}

InstrList *function2instr(Function *fun){
  Instr *instr = mk_instr_lbl(currLabel++);
  InstrList *next = cmdlist2instr(fun->cmdlist);
  InstrList *res = mk_instrlist(instr,next);

  return res;
}

void print_syscall(int number){
  printf("\taddi $v0, $0, %d\n",number);
  printf("\tSyscall\n");
}

void print_enter(){
  printf("\taddi $a0, $0, '\\n'\n");
  print_syscall(11);
}

void print_loadfromsp(char *reg){
  printf("\tlw %s, 4($sp)\n",reg);
  printf("\taddi $sp, $sp, 4\n");
}

void print_storeinsp(char *reg){
  printf("\tsw %s, 0($sp)\n",reg);
  printf("\taddi $sp, $sp, -4\n");
}

void print_comparation(char *type){
  int comp0 = currLabel; currLabel++;
  int compf = currLabel; currLabel++;
  print_loadfromsp("$t1");
  print_loadfromsp("$t2");
  printf("\t%s $t1, $t2, L%d\n",type,comp0);
  printf("\taddi $t1, $0, 0\n");
  print_storeinsp("$t1");
  printf("\tj L%d\n",compf);
  printf("L%d:\n",comp0);
  printf("\taddi $t1, $0, 1\n");
  print_storeinsp("$t1");
  printf("L%d:\n",compf);
}

void print_comparation_sub(char *type){
  int comp0 = currLabel; currLabel++;
  int compf = currLabel; currLabel++;
  print_loadfromsp("$t1");
  print_loadfromsp("$t2");
  printf("\tsub $t2, $t2, $t1\n");
  printf("\t%s $t2 L%d\n",type,comp0);
  printf("\taddi $t1, $0, 0\n");
  print_storeinsp("$t1");
  printf("\tj L%d\n",compf);
  printf("L%d:\n",comp0);
  printf("\taddi $t1, $0, 1\n");
  print_storeinsp("$t1");
  printf("L%d:\n",compf);
}

void printMips(InstrList *instrlist)
{
  InstrList* tmp = instrlist;
  printf("\t.data\n");

  while(tmp){
    if(tmp->node->type != E_DCL_VAR){
      tmp = tmp->next;
      continue;
    }

    printf("%s:\t.space 4\n",tmp->node->attr.var);

    tmp = tmp->next;
  }
  printf("\n");

  printf("\t.text\n\n");
  tmp = instrlist;
  while (tmp)
  {
    switch (tmp->node->type){
      case E_LDC:
        printf("\taddi $t1, $0, %d\n",tmp->node->attr.num);
        print_storeinsp("$t1");
        break;
      case E_LOD:
        printf("\tlw $t1, %s\n",tmp->node->attr.var);
        print_storeinsp("$t1");
        break;
      case E_STO:
        print_loadfromsp("$t1");
        printf("\tsw $t1, %s\n",tmp->node->attr.var);
        break;
      case E_ADI:
        print_loadfromsp("$t1");
        print_loadfromsp("$t2");
        printf("\tadd $t2, $t1, $t2\n");
        print_storeinsp("$t2");
        break;
      case E_SBI:
        print_loadfromsp("$t1");
        print_loadfromsp("$t2");
        printf("\tsub $t2, $t2, $t1\n");
        print_storeinsp("$t2");
        break;
      case E_MOD:
        print_loadfromsp("$t1");
        print_loadfromsp("$t2");
        printf("\tdiv $t1, $t2\n");
        printf("\tmfhi $t2\n");
        print_storeinsp("$t2");
        break;
      case E_MPI:
        print_loadfromsp("$t1");
        print_loadfromsp("$t2");
        printf("\tmult $t1, $t2\n");
        printf("\tmflo $t2\n");
        print_storeinsp("$t2");
        break;
      case E_DVI:
        print_loadfromsp("$t1");
        print_loadfromsp("$t2");
        printf("\tdiv $t1, $t2\n");
        printf("\tmflo $t2\n");
        print_storeinsp("$t2");
        break;
      case E_EQU:
        print_comparation("beq");
        break;
      case E_GEQ:
        print_comparation_sub("bgez");
        break;
      case E_GES:
        print_comparation_sub("bgtz");
        break;
      case E_LEQ:
        print_comparation_sub("blez");
        break;
      case E_LES:
        print_comparation_sub("bltz");
        break;
      case E_NEQ:
        print_comparation("bne");
        break;
      case E_FJP:
        print_loadfromsp("$t1");
        printf("\tbeq $t1, $0, L%d\n",tmp->node->attr.num);
        break;
      case E_UJP:
        printf("\tj L%d\n",tmp->node->attr.num);
        break;
      case E_READ:
        print_syscall(5);
        printf("\tsw $v0, %s\n",tmp->node->attr.var);
        break;
      case E_WRT:
        printf("\tlw $a0, %s\n",tmp->node->attr.var);
        print_syscall(1);
        print_enter();
        break;
      case E_LBL:
        printf("L%d:\n",tmp->node->attr.num);
        break;
      default :
      break;
    }

    tmp = tmp->next;
  }

  print_syscall(10);
  return;
}

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

    int fdout = open("out.pcode", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    dup2(fdout, STDOUT_FILENO);
    close(fdout);

    print_InstrList(res);
    fflush(stdout);

    fdout = open("out.mips", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    dup2(fdout, STDOUT_FILENO);
    close(fdout);

    printMips(res);
    fflush(stdout);
  }
  return 0;
}
