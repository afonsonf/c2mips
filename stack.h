// Stack definitions
#ifndef __stack_h__
#define __syack_h__

typedef enum {
    E_LDC, //load constant (-,x) Q
    E_LOD, //load content of address (-,x) PQ
    E_LCA, //load adress (-,a) Q

    E_STO, //store content in adress (ax,-) -

    E_ADI, //sum (ii,i) -
    E_SBI, //subtr (ii,i) -
    E_MOD, //mod (ii,i) -
    E_MPI, //mult (ii,i) -
    E_DVI, //div (ii,i) -

    E_EQU, //compare on equal (xx,b) Q
    E_GEQ, //compare on greater or equal (xx,b) Q
    E_GES, //compare on greater (xx,b) Q
    E_LEQ, //compare on less than or equal (xx,b) Q
    E_LES, //compare on less than (xx,b) Q
    E_NEQ, //compare on not equal (xx,b) Q

    E_IOR, //logic OR (bb,b) -
    E_AND, //logic AND (bb,b) -
    E_NOT, //logic NOT (b,b) -

    E_FJP, //false jump (b,-) Q
    E_UJP, //unconditional jump (-,-) Q

    E_LBL, //label
    E_DCL_VAR //declaration of var
} instrtype;

struct _Instr{
    instrtype type;
    union {
        int num;
        char* var;
        char* lbl;
    } attr;
};

struct _InstrList{
    struct _Instr *node;
    struct _InstrList *next;
};

typedef struct _Instr Instr;
typedef struct _InstrList InstrList;

Instr* mk_instr_ldc(int num);
Instr* mk_instr_lod(char* var);
Instr* mk_instr_lca(char *var);
Instr* mk_instr_lbl(char *lbl);
Instr* mk_instr_dcl_var(char *var);
Instr* mk_instr(instrtype type);

InstrList* mk_instrlist(Instr* instr, InstrList* next);
Instr* instrlist_head(InstrList* instrlist);
InstrList* instrlist_tail(InstrList* instrlist);
void instrlist_append(InstrList* instrlist1, InstrList* instrlist2);
void instrlist_append_instr(InstrList* instrlist, Instr *instr);

void print_Instr(Instr* instr);
void print_InstrList(InstrList *instrlist);

#endif
