#ifndef CLANMO_VM_H
#define CLANMO_VM_H

#include <stdbool.h>
#include <stdint.h>

#define LM_INTEGER 1
#define LM_STRING 2
#define LM_IDENTIFIER 3
#define LM_FUNCTION 4
#define LM_NONE 5
#define LM_BOOLEAN 6
#define LM_OBJECT 7
#define LM_LIST 8
#define LM_FLOAT 9

#define OP_PUSH 1
#define OP_POP 2
#define OP_BIN 3
#define OP_WRITE 4
#define OP_CALL 5
#define OP_HALT 6
#define OP_RET 7
#define OP_JUMP 8
#define OP_JUMP_IF_FALSE 9
#define OP_DUP 10
#define OP_STORE 11
#define OP_LOAD 12
#define OP_MAKE_LIST 13
#define OP_GET_INDEX 14
#define OP_SET_INDEX 15
#define OP_UNARY 16
#define OP_NEW_OBJ 17
#define OP_GET_FIELD 18
#define OP_SET_FIELD 19

#define UNA_MINUS 1
#define UNA_BANG 2

#define BIN_OP_ADD 1
#define BIN_OP_SUB 2
#define BIN_OP_MUL 3
#define BIN_OP_DIV 4
#define BIN_OP_MOD 5

#define BIN_OP_EEQ 6
#define BIN_OP_NEQ 7
#define BIN_OP_GEQ 8
#define BIN_OP_GTN 9
#define BIN_OP_LEQ 10
#define BIN_OP_LTN 11

#define BIN_OP_AND 12
#define BIN_OP_OR  13

struct Value;
typedef struct Value (*NativeFunction)(int argc, struct Value *args);

typedef struct {
    uint8_t op_code;
    uint16_t value;
} OpCode;

typedef struct {
    char *name;
    uint8_t args_count;
    uint32_t local_count;
    OpCode *body;
    NativeFunction native_function;
    bool is_native;
} Function;

typedef struct {
    uint32_t length;
    char *string;
} String;

typedef struct Value {
    int type;
    union {
        uint32_t integer;
        double floating;
        String *string;
        bool boolean;
        Function *function;
    } as;
} Value;

typedef struct {
    Value *symbol_table;
} VMContext;

static Value make_none(void) {
    return (Value){ .type = LM_NONE };
}

static Value make_bool(const bool b) {
    return (Value){
        .type = LM_BOOLEAN,
        .as.boolean = b
    };
}

static Value make_int(const uint32_t i) {
    return (Value){
        .type = LM_INTEGER,
        .as.integer = i
    };
}

static Value make_float(const double d) {
    return (Value){
        .type = LM_FLOAT,
        .as.floating = d
    };
}

static Value make_string(String *s) {
    return (Value){
        .type = LM_STRING,
        .as.string = s
    };
}

// static inline Value make_list(List *l) {
//     return (Value){
//         .type = VALUE_LIST,
//         .as.list = l
//     };
// }
//
// static inline Value make_object(Object *o) {
//     return (Value){
//         .type = VALUE_OBJECT,
//         .as.object = o
//     };
// }

static Value make_function(Function *f) {
    return (Value){
        .type = LM_FUNCTION,
        .as.function = f
    };
}

#endif
