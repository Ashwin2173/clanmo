#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <windows.h>

#include "../include/error.h"
#include "../include/list.h"
#include "../include/value.h"

LM_Value native_print(const size_t argc, const LM_Value* argv) {
    for (int i = 0; i < argc; ++i) {
        LM_Value value = argv[i];
        switch (value.type) {
            case LM_INTEGER:
                printf("%"PRId64, value.as.integer);
                break;
            case LM_STRING:
                printf("%s", value.as.string->string);
                break;
            case LM_FUNCTION: {
                const LM_Function *function = value.as.function;
                if (function->is_native)
                    printf("<native_function(%s) at %p>", function->name, (void*) function);
                else
                    printf("<function(%s) at %p>", function->name, (void*) function);
                break;
            }
            case LM_LIST: {
                const LM_List *list = value.as.list;
                printf("[");
                for (size_t item = 0; item < list->length; ++item) {
                    native_print(1, &list->values[item]);
                    if (item != list->length - 1) {
                        printf(", ");
                    }
                }
                printf("]");
                break;
            }
            case LM_BOOLEAN: {
                const bool boolean = value.as.boolean;
                if (boolean)
                    printf("true");
                else
                    printf("false");
                break;
            }
            case LM_NONE:
                printf("null");
                break;
            default:
                printf("<value:%d at %p>", value.type, (void*) &value);
        }
    }
    return make_none();
}

LM_Value native_input(const size_t argc, LM_Value* argv) {
    native_print(argc, argv);
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return make_none();
    }
    input[strcspn(input, "\n")] = '\0';
    LM_String *s = malloc(sizeof(LM_String));
    s->length = strlen(input);
    s->string = malloc(s->length + 1);
    strcpy(s->string, input);
    return make_string(s);
}

LM_Value native_now(const size_t argc, LM_Value* argv) {
    (void) argc; (void) argv;
    static LARGE_INTEGER freq;
    static int init = 0;
    if (!init) {
        QueryPerformanceFrequency(&freq);
        init = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    const int64_t us = (counter.QuadPart * 1000000LL) / freq.QuadPart;
    return make_int(us);
}

LM_Value native_len(const size_t argc, LM_Value* argv) {
    if (argc == 1) {
        if (argv[0].type == LM_LIST) {
            return make_int((int64_t) argv[0].as.list->length);
        }
        if (argv[0].type == LM_STRING) {
            return make_int((int64_t) argv[0].as.string->length);
        }
        Fault(TYPE_ERROR, "Invalid type for len()");
    }
    return make_int(0);
}