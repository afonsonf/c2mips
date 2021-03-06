#include <stdlib.h> // for malloc
#include <stdio.h> //for printf
#include "stack.h"

Instr* mk_instr_ldc(int num){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_LDC;
  node->attr.num = num;
  return node;
}

Instr* mk_instr_lod(char* var){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_LOD;
  node->attr.var = var;
  return node;
}

Instr* mk_instr_sto(char* var){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_STO;
  node->attr.var = var;
  return node;
}

Instr* mk_instr_ujp(int lbl){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_UJP;
  node->attr.num = lbl;
  return node;
}

Instr* mk_instr_fjp(int lbl){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_FJP;
  node->attr.num = lbl;
  return node;
}

Instr* mk_instr_read(char *var){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_READ;
  node->attr.var = var;
  return node;
}

Instr* mk_instr_wrt(char *var){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_WRT;
  node->attr.var = var;
  return node;
}

Instr* mk_instr_lbl(int lbl){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_LBL;
  node->attr.num = lbl;
  return node;
}

Instr* mk_instr_dcl_var(char *var){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = E_DCL_VAR;
  node->attr.var = var;
  return node;
}

Instr* mk_instr(instrtype type){
  Instr *node = (Instr*) malloc(sizeof(Instr));
  node->type = type;
  return node;
}

InstrList* mk_instrlist(Instr* instr, InstrList* next){
  InstrList *node = (InstrList*) malloc(sizeof(InstrList));
  node->node = instr;
  node->next = next;
  return node;
}

Instr* instrlist_head(InstrList* instrlist){
  return instrlist->node;
}

InstrList* instrlist_tail(InstrList* instrlist){
  return instrlist->next;
}

void instrlist_append(InstrList* instrlist1, InstrList* instrlist2){  
  InstrList *tmp = instrlist1;

  while(tmp->next) tmp = tmp->next;

  tmp->next = instrlist2;
}

void instrlist_append_instr(InstrList* instrlist, Instr* instr){
  InstrList *tmp = instrlist;

  while(tmp->next) tmp = tmp->next;

  tmp->next = mk_instrlist(instr, NULL);
}

void print_Instr(Instr* instr){
  //if(!instr) return;
  switch(instr->type){
    case E_LDC:
      printf("LDC %d\n",instr->attr.num);
      break;
    case E_LOD:
      printf("LOD %s\n",instr->attr.var);
      break;
    case E_STO:
      printf("STO %s\n",instr->attr.var);
      break;
    case E_ADI:
      printf("ADI\n");
      break;
    case E_SBI:
      printf("SBI\n");
      break;
    case E_MOD:
      printf("MOD\n");
      break;
    case E_MPI:
      printf("MPI\n");
      break;
    case E_DVI:
      printf("DVI\n");
      break;
    case E_EQU:
      printf("EQU\n");
      break;
    case E_GEQ:
      printf("GEQ\n");
      break;
    case E_GES:
      printf("GES\n");
      break;
    case E_LEQ:
      printf("LEQ\n");
      break;
    case E_LES:
      printf("LES\n");
      break;
    case E_NEQ:
      printf("NEQ\n");
      break;
    case E_FJP:
      printf("FJP L%d\n",instr->attr.num);
      break;
    case E_UJP:
      printf("UJP L%d\n",instr->attr.num);
      break;
    case E_LBL:
      printf("L%d\n",instr->attr.num);
      break;
    case E_DCL_VAR:
      printf("%s\n",instr->attr.var);
      break;
    case E_READ:
      printf("READ %s\n",instr->attr.var);
      break;
    case E_WRT:
      printf("WRT %s\n", instr->attr.var);
      break;
  }
}

void print_InstrList(InstrList *instrlist){
  if(instrlist){
    print_Instr(instrlist->node);
    print_InstrList(instrlist->next);
  }
}
