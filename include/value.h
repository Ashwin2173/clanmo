#ifndef CLANMO_VALUE_H
#define CLANMO_VALUE_H

#include <stdint.h>
#include <stdbool.h>

#include "types.h"

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

static LM_Value make_list(LM_List *list) {
    return (LM_Value) {
        .type = LM_LIST,
        .as.list = list
    };
}

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