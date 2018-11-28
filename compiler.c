#include <stdio.h>
#include "parser.h"
#include "stack.h"

int currLabel;

InstrList* expr2instr(Expr* expr){
  if(expr->kind == E_INTEGER){
    Instr *no = mk_instr_ldc_int(expr->attr.value);
    return mk_instrlist(no,NULL);
  }
  else if(expr->kind == E_VAR){
    Instr *no = mk_instr_ldc_var(expr->attr.var->name);
    return mk_instrlist(no,NULL);
  }
  else if(expr->kind == E_OPERATION){
    Instr *no;
    switch (expr->attr.op.operator) {
      case PLUS:
        no = mk_instr(E_ADI);
        break;
      case MINUS:
        no = mk_instr(E_SBI);
        break;
      case MULT:
        no = mk_instr(E_MPI);
        break;
    }

    InstrList* instrlist = expr2instr(expr->attr.op.left);
    append(instrlist,expr2instr(expr->attr.op.right));
    append(instrlist,mk_instrlist(no,NULL));
    
    return instrlist;
  }
}

InstrList* atrib2instr(Attrib* atrib){

}

InstrList* cmdlist2instr(CmdList* cmdlist){
  if(!cmdlist) return NULL;
  InstrList *next = cmdlist2instr(cmdlist->next);
  InstrList *node = cmd2instr() 
}

/*
void printMips(InstrList* instrlist){
  if(instrlist){
    switch(instrlist->node->type){
      case E_LDC:
        printf("addi $t0 $0 %d\n",instrlist->node->attr.num);
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
}
*/

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) {
    currLabel = 0;
    //printFunction(root,0);
  }
  return 0;
}
