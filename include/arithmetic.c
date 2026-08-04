#include <math.h>

#include "vm.h"
#include "error.h"

#define BIN_NPE_CHECK(left, right)                                              \
   if (left->type == LM_NONE || right->type == LM_NONE) {                       \
      Fault(NULL_POINTER_EXCEPTION, "Null pointer exception");                  \
   }                                                                            \

#define BIN_NUMERIC_OP(left, right, OP)                                         \
   if ((left)->type == LM_FLOAT || (right)->type == LM_FLOAT) {                 \
      const double lhs = (left)->type == LM_FLOAT                               \
          ? (left)->as.floating                                                 \
          : (double)(left)->as.integer;                                         \
      const double rhs = (right)->type == LM_FLOAT                              \
          ? (right)->as.floating                                                \
          : (double)(right)->as.integer;                                        \
      (left)->type = LM_FLOAT;                                                  \
      (left)->as.floating = lhs OP rhs;                                         \
   } else if ((left)->type == LM_INTEGER && (right)->type == LM_INTEGER) {      \
      (left)->as.integer = (left)->as.integer OP (right)->as.integer;           \
   } else {                                                                     \
      Fault(TYPE_ERROR, "unsupported operand types for " #OP);                  \
   }                                                                            \

#define BIN_COMPARE_OP(left, right, OP)                                         \
   if ((left)->type == LM_FLOAT || (right)->type == LM_FLOAT) {                 \
      const double lhs = (left)->type == LM_FLOAT                               \
          ? (left)->as.floating                                                 \
          : (double)(left)->as.integer;                                         \
      const double rhs = (right)->type == LM_FLOAT                              \
          ? (right)->as.floating                                                \
          : (double)(right)->as.integer;                                        \
      (left)->type = LM_BOOLEAN;                                                \
      (left)->as.boolean = (lhs OP rhs);                                        \
   } else if ((left)->type == LM_INTEGER && (right)->type == LM_INTEGER) {      \
      (left)->type = LM_BOOLEAN;                                                \
      (left)->as.boolean = ((left)->as.integer OP (right)->as.integer);         \
   } else {                                                                     \
      Fault(TYPE_ERROR, "unsupported operand types for " #OP);                  \
   }                                                                            \

#define BIN_BOOL_OP(left, right, OP)                                            \
     if ((left)->type != LM_BOOLEAN ||                                          \
         (right)->type != LM_BOOLEAN) {                                         \
         Fault(TYPE_ERROR, "Unsupported boolean operands");                     \
     }                                                                          \
     (left)->as.boolean = ((left)->as.boolean OP (right)->as.boolean);          \
     (left)->type = LM_BOOLEAN;                                                 \

void op_add(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_NUMERIC_OP(left, right, +);
}

void op_sub(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_NUMERIC_OP(left, right, -);
}

void op_mul(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_NUMERIC_OP(left, right, *);
}

void op_div(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_NUMERIC_OP(left, right, /);
}

void op_mod(LM_Value *left, const LM_Value *right) {
   if (left->type == LM_FLOAT || right->type == LM_FLOAT) {
      const double lhs = left->type == LM_FLOAT ? left->as.floating: (double)left->as.integer;
      const double rhs = right->type == LM_FLOAT ? right->as.floating : (double)right->as.integer;
      left->type = LM_FLOAT;
      left->as.floating = fmod(lhs, rhs);
   } else if (left->type == LM_INTEGER && right->type == LM_INTEGER) {
      left->as.integer %= right->as.integer;
   } else {
      Fault(TYPE_ERROR, "unsupported operand types for %");
   }
}

void op_eeq(LM_Value *left, const LM_Value *right) {
   if (left->type == LM_NONE || right->type == LM_NONE) {
      left->as.boolean = left->type == right->type;
      left->type = LM_BOOLEAN;
      return;
   }
   BIN_COMPARE_OP(left, right, ==);
}

void op_neq(LM_Value *left, const LM_Value *right) {
   if (left->type == LM_NONE || right->type == LM_NONE) {
      left->as.boolean = left->type != right->type;
      left->type = LM_BOOLEAN;
      return;
   }
   BIN_COMPARE_OP(left, right, !=);
}

void op_geq(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_COMPARE_OP(left, right, >=);
}

void op_gtn(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_COMPARE_OP(left, right, >);
}

void op_leq(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_COMPARE_OP(left, right, <=);
}

void op_ltn(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_COMPARE_OP(left, right, <);
}

void op_and(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_BOOL_OP(left, right, &&);
}
void op_or(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   BIN_BOOL_OP(left, right, ||);
}
