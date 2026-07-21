#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

Value native_print(const int argc, Value* argv) {
    for (int i = 0; i < argc; ++i) {
        Value value = argv[i];
        switch (value.type) {
            case LM_INTEGER: printf("%d", value.as.integer); break;
            case LM_STRING: printf("%s", value.as.string->string); break;
            default: printf("<value:%d at %p>", value.type, (void*) &value); break;
        }
    }
    return make_none();
}

Value native_input(const int argc, Value* argv) {
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return make_none();
    }
    input[strcspn(input, "\n")] = '\0';
    String *s = malloc(sizeof(String));
    s->length = strlen(input);
    s->string = malloc(s->length + 1);
    strcpy(s->string, input);
    return make_string(s);
}