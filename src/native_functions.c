#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../include/value.h"

LM_Value native_print(const size_t argc, const LM_Value* argv) {
    for (int i = 0; i < argc; ++i) {
        LM_Value value = argv[i];
        switch (value.type) {
            case LM_INTEGER:
                printf("%"PRIu64, value.as.integer);
                break;
            case LM_STRING:
                printf("%s", value.as.string->string);
                break;
            default:
                printf("<value:%d at %p>", value.type, (void*) &value);
        }
    }
    return make_none();
}

LM_Value native_input(const size_t argc, LM_Value* argv) {
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