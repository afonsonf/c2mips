#include <stdio.h>
#include "parser.h"
#include "stack.h"

InstrList* compile(Expr* expr){
  if(expr->kind == E_INTEGER){
    Instr *no = mk_instr_ldc(expr->attr.value);
    return mk_instrlist(no,NULL);
  }
  else if(expr->kind == E_OPERATION){
    Instr *no;
    switch (expr->attr.op.operator) {
      case PLUS:
        no = mk_instr_adi();
        break;
      case MINUS:
        no = mk_instr_sbi();
        break;
      case MULT:
        no = mk_instr_mpi();
        break;
    }

    InstrList* instrlist = compile(expr->attr.op.left);
    append(instrlist,compile(expr->attr.op.right));
    append(instrlist,mk_instrlist(no,NULL));
    
    return instrlist;
  }
}

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
      //printFunction(root,0);
  }
  return 0;
}
