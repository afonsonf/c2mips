// Stack definitions
#ifndef __stack_h__
#define __syack_h__

struct _Instr{
    enum {
        E_LDC,
        E_ADI,
        E_MPI,
        E_SBI
    } type;
    union {
        int num;
        char* var;
    } attr;
};

struct _InstrList{
    struct _Instr *node;
    struct _InstrList *next;
};

typedef struct _Instr Instr;
typedef struct _InstrList InstrList;

Instr* mk_instr_ldc_int(int num);
Instr* mk_instr_ldc_var(char* var);
Instr* mk_instr_adi();
Instr* mk_instr_mpi();
Instr* mk_instr_sbi();

InstrList* mk_instrlist(Instr* instr, InstrList* next);
Instr* instrlist_head(InstrList* instrlist);
InstrList* instrlist_tail(InstrList* instrlist);
void append(InstrList* instrlist1, InstrList* instrlist2);

void print_Instr(Instr* instr);
void print_InstrList(InstrList *instrlist);

#endif

