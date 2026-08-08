#ifndef CLANMO_ARITHMETIC_H
#define CLANMO_ARITHMETIC_H

#define OP_COUNT 13

typedef void (*LM_Bin_Arth)(LM_Value *left, const LM_Value *right);
typedef void (*LM_Un_Arth)(LM_Value *value);

void op_add(LM_Value *left, const LM_Value *right);
void op_sub(LM_Value *left, const LM_Value *right);
void op_mul(LM_Value *left, const LM_Value *right);
void op_div(LM_Value *left, const LM_Value *right);
void op_mod(LM_Value *left, const LM_Value *right);
void op_eeq(LM_Value *left, const LM_Value *right);
void op_neq(LM_Value *left, const LM_Value *right);
void op_geq(LM_Value *left, const LM_Value *right);
void op_gtn(LM_Value *left, const LM_Value *right);
void op_leq(LM_Value *left, const LM_Value *right);
void op_ltn(LM_Value *left, const LM_Value *right);
void op_and(LM_Value *left, const LM_Value *right);
void op_or(LM_Value *left, const LM_Value *right);

void op_minus(LM_Value *value);
void op_bang(LM_Value *value);

LM_Bin_Arth binop_dispatch_table[] = {
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,
    op_eeq,
    op_neq,
    op_geq,
    op_gtn,
    op_leq,
    op_ltn,
    op_and,
    op_or
};

LM_Un_Arth unop_dispatch_table[] = {
    op_minus,
    op_bang
};

#endif
