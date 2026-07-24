#ifndef CLANMO_VALUE_H
#define CLANMO_VALUE_H

#include <stdbool.h>
#include <stdint.h>

enum LM_DataType {
    LM_INTEGER = 1,
    LM_STRING = 2,
    LM_IDENTIFIER = 3,
    LM_FUNCTION = 4,
    LM_NONE = 5,
    LM_BOOLEAN = 6,
    LM_BUILT_IN_FUNCTION = 7,
    LM_FLOAT = 8
};

enum LM_OPCode {
    OP_PUSH          = 1,
    OP_POP           = 2,
    OP_BIN           = 3,
    OP_WRITE         = 4,
    OP_CALL          = 5,
    OP_HALT          = 6,
    OP_RET           = 7,
    OP_JUMP          = 8,
    OP_JUMP_IF_FALSE = 9,
    OP_DUP           = 10,
    OP_STORE         = 11,
    OP_LOAD          = 12,
    OP_MAKE_LIST     = 13,
    OP_GET_INDEX     = 14,
    OP_SET_INDEX     = 15,
    OP_UNARY         = 16,
    OP_NEW_OBJ       = 17,
    OP_GET_FIELD     = 18,
    OP_SET_FIELD     = 19
};

enum LM_BIN_TYPE {
    BIN_OP_ADD = 1,
    BIN_OP_LTN = 11
};

typedef struct {
    uint64_t length;
    char *string;
} LM_String;

typedef struct {
    enum LM_OPCode op_code;
    uint16_t value;
} LM_OpCode;

struct LM_Value;
typedef struct LM_Value (*NativeFunction)(size_t argc, struct LM_Value *args);

typedef struct {
    char *name;
    uint8_t args_count;
    uint32_t local_count;
    LM_OpCode *body;
    NativeFunction native_function;
    bool is_native;
} LM_Function;

typedef struct LM_Value {
    enum LM_DataType type;
    union {
        int64_t integer;
        double floating;
        LM_String *string;
        bool boolean;
        LM_Function *function;
    } as;
} LM_Value;

static LM_Value make_none(void) {
    return (LM_Value) { .type = LM_NONE };
}

static LM_Value make_bool(const bool b) {
    return (LM_Value) {
        .type = LM_BOOLEAN,
        .as.boolean = b
    };
}

static LM_Value make_int(const int64_t i) {
    return (LM_Value) {
        .type = LM_INTEGER,
        .as.integer = i
    };
}

static LM_Value make_float(const double d) {
    return (LM_Value) {
        .type = LM_FLOAT,
        .as.floating = d
    };
}

static LM_Value make_string(LM_String *s) {
    return (LM_Value) {
        .type = LM_STRING,
        .as.string = s
    };
}

static LM_Value make_boolean(const bool b) {
    return (LM_Value) {
        .type = LM_BOOLEAN,
        .as.boolean = b
    };
}

// static inline LM_Value make_list(List *l) {
//     return (LM_Value) {
//         .type = VALUE_LIST,
//         .as.list = l
//     };
// }
//
// static inline LM_Value make_object(Object *o) {
//     return (LM_Value) {
//         .type = VALUE_OBJECT,
//         .as.object = o
//     };
// }

static LM_Value make_function(LM_Function *f) {
    return (LM_Value) {
        .type = LM_FUNCTION,
        .as.function = f
    };
}

#endif