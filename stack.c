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

Instr* mk_instr_lca(char* var){
    Instr *node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LCA;
    node->attr.var = var;
    return node;
}

Instr* mk_instr_lbl(char *lbl){
    Instr *node = (Instr*) malloc(sizeof(Instr));
    node->type = E_LBL;
    node->attr.lbl = lbl;
    return node;
}

Instr* mk_instr_dcl_var(char *var){
    Instr *node = (Instr*) malloc(sizeof(Instr));
    node->type = E_DCL_VAR;
    node->attr.lbl = var;
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
    switch(instr->type){
        case E_LDC:
            printf("LDC %d\n",instr->attr.num);
            break;
        case E_ADI:
            printf("ADI\n");
            break;
        case E_MPI:
            printf("MPI\n");
            break;
        case E_SBI:
            printf("SBI\n");
            break;
    }
}

void print_InstrList(InstrList *instrlist){
    if(instrlist){
        print_Instr(instrlist->node);
        print_InstrList(instrlist->next);
    }
}
