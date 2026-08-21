#include <math.h>
#include <inttypes.h>
#include <string.h>

#include "vm.h"
#include "error.h"

#define UN_NPE_CHECK(value)                                                     \
   if (value->type == LM_NONE) {                                                \
      Fault(NULL_POINTER_EXCEPTION, "Null pointer exception");                  \
   }                                                                            \

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

char* to_string(const LM_Value *value) {
   char buffer[64];
   switch (value->type) {
   case LM_STRING:
      return strdup(value->as.string->string);
   case LM_INTEGER:
      snprintf(buffer, sizeof(buffer), "%" PRId64, value->as.integer);
      return strdup(buffer);
   case LM_FLOAT:
      snprintf(buffer, sizeof(buffer), "%g", value->as.floating);
      return strdup(buffer);
   case LM_BOOLEAN:
      return strdup(value->as.boolean ? "true" : "false");
   case LM_NONE:
      return strdup("null");
   case LM_FUNCTION:
      return strdup("<function>");
   case LM_OBJECT:
      return strdup("<object>");
   default:
      Fault(CORE_FAULT, "Unsupported value in to_string()");
   }
}

void op_add(LM_Value *left, const LM_Value *right) {
   BIN_NPE_CHECK(left, right);
   if (left->type == LM_STRING || right->type == LM_STRING) {
      char* lhs = to_string(left);
      char* rhs = to_string(right);
      const size_t lhs_len = strlen(lhs);
      const size_t rhs_len = strlen(rhs);
      LM_String *str = malloc(sizeof(*str));
      str->length = lhs_len + rhs_len;
      str->string = malloc(str->length + 1);
      memcpy(str->string, lhs, lhs_len);
      memcpy(str->string + lhs_len, rhs, rhs_len);
      str->string[str->length] = '\0';
      free(lhs);
      free(rhs);
      left->type = LM_STRING;
      left->as.string = str;
      return;
   }
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
   if (right->type == LM_INTEGER && right->as.integer == 0 ||
       right->type == LM_FLOAT && right->as.floating == 0) {
      Fault(DIVIDE_BY_ZERO, "Divide by zero");
   }
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
   bool result = false;
   if (left->type == LM_NONE || right->type == LM_NONE) {
      result = left->type == right->type;
   } else if ((left->type == LM_INTEGER || left->type == LM_FLOAT) &&
            (right->type == LM_INTEGER || right->type == LM_FLOAT)) {
      const double lhs = left->type == LM_FLOAT
          ? left->as.floating
          : (double)left->as.integer;
      const double rhs = right->type == LM_FLOAT
          ? right->as.floating
          : (double)right->as.integer;
      result = lhs == rhs;
   } else if (left->type == LM_BOOLEAN && right->type == LM_BOOLEAN) {
      result = left->as.boolean == right->as.boolean;
   } else if (left->type == LM_STRING && right->type == LM_STRING) {
      result = strcmp(left->as.string->string, right->as.string->string) == 0;
   } else {
      result = false;
   }
   left->as.boolean = result;
   left->type = LM_BOOLEAN;
}

void op_neq(LM_Value *left, const LM_Value *right) {
   op_eeq(left, right);
   left->as.boolean = !left->as.boolean;
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

void op_minus(LM_Value *value) {
   UN_NPE_CHECK(value);
   if (value->type == LM_INTEGER) {
      value->as.integer = -value->as.integer;
   } else if (value->type == LM_FLOAT) {
      value->as.floating = -value->as.floating;
   } else {
      Fault(TYPE_ERROR, "unsupported operand types for unary -");
   }
}

void op_bang(LM_Value *value) {
   UN_NPE_CHECK(value);
   if (value->type == LM_BOOLEAN) {
      value->as.boolean = !value->as.boolean;
   } else {
      Fault(TYPE_ERROR, "unsupported operand types for unary !");
   }
}
