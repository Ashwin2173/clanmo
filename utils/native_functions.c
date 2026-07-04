#include <stdio.h>
#include "vm.h"

Value native_print(const int argc, Value* argv) {
    for (int i = 0; i < argc; ++i) {
        Value value = argv[i];
        switch (value.type) {
            case LM_INTEGER: printf("%d", value.as.integer); break;
            case LM_STRING: printf("%p", &value.as.string); break;
            default: printf("<value:%d at %p>", value.type, (void*) &value); break;
        }
    }
    return make_none();
}
