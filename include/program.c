#include "program.h"

#include <stdio.h>
#include <stdlib.h>

void program_gc(Program *program) {
    for (size_t i = 0; i < program->symbol_count; i++) {
        LM_Value *value = &program->symbol_table[i];
        switch (value->type) {
            case LM_STRING:
                free(value->as.string->string);
                free(value->as.string);
                break;
            case LM_MEMBER:
                free(value->as.member->name);
                free(value->as.member);
                break;
            case LM_FUNCTION:
                free(value->as.function->name);
                free(value->as.function->body);
                free(value->as.function);
                break;
            case LM_NONE:
            case LM_BOOLEAN:
            case LM_INTEGER:
                break;
            default:
                fprintf(stderr,"\n[WARN] Unhandled symbol garbage collection type: %d", value->type);
                break;
        }
    }
    free(program->symbol_table);
    free(program);
}
