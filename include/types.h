#ifndef CLANMO_TYPES_H
#define CLANMO_TYPES_H

#include <stdint.h>
#include <stdbool.h>

enum LM_DataType {
    LM_INTEGER = 1,
    LM_STRING = 2,
    LM_MEMBER = 3,
    LM_FUNCTION = 4,
    LM_NONE = 5,
    LM_BOOLEAN = 6,
    LM_BUILT_IN_FUNCTION = 7,
    LM_FLOAT = 8,

    // the following are dataType not symbolType
    LM_LIST = 9,
    LM_OBJECT = 10
};

enum LM_OPCode {
    OP_PUSH          = 1,
    OP_POP           = 2,
    OP_BIN           = 3,
    OP_WRITE         = 4,
    OP_CALL          = 5,
    OP_HALT          = 6,   // ignore
    OP_RET           = 7,
    OP_JUMP          = 8,
    OP_JUMP_IF_FALSE = 9,
    OP_DUP           = 10,  // ignore
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
    BIN_OP_SUB = 2,
    BIN_OP_MUL = 3,
    BIN_OP_DIV = 4,
    BIN_OP_MOD = 5,
    BIN_OP_EEQ = 6,
    BIN_OP_NEQ = 7,
    BIN_OP_GEQ = 8,
    BIN_OP_GTN = 9,
    BIN_OP_LEQ = 10,
    BIN_OP_LTN = 11,
    BIN_OP_AND = 12,
    BIN_OP_OR  = 13,
};

enum LM_UN_TYPE {
    UNARY_MINUS = 1,
    UNARY_BANG  = 2
};

struct LM_Value;
typedef struct LM_List LM_List;
typedef struct LM_Object LM_Object;
typedef struct LM_Value (*NativeFunction)(size_t argc, struct LM_Value *args);

typedef struct {
    uint64_t length;
    char *string;
} LM_String;

typedef struct {
    char *name;
} LM_Member;

typedef struct {
    enum LM_OPCode op_code;
    uint16_t value;
} LM_OpCode;

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
        bool boolean;
        double floating;
        int64_t integer;
        LM_List *list;
        LM_String *string;
        LM_Member *member;
        LM_Object *object;
        LM_Function *function;
    } as;
} LM_Value;

#endif
