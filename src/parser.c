#include <stdio.h>
#include <string.h>

#include "../include/vm.h"
#include "../include/parser.h"
#include "native_functions.h"

#define MAGIC 2273
#define MAJOR 1
#define MINOR 0

#define INIT_POINT "main"

void validate_header(FILE *file) {
    if (next_int4(file) != MAGIC || next_int2(file) != MAJOR || next_int2(file) != MINOR) {
        Fault(INIT_FAULT, "Incorrect file header");
    }
}

void load_native_function(LM_Function *function) {
    const char *fn_name = function->name;
    if (strcmp(fn_name, "print") == 0) {
        function->is_native = true;
        function->native_function = native_print;
    } else if (strcmp(fn_name, "input") == 0) {
        function->is_native = true;
        function->native_function = native_input;
    }
}

LM_Value *read_symbols(FILE *file, Program *program) {
    const uint16_t symbol_count = next_int2(file);
    LM_Value *symbols = malloc(sizeof(LM_Value) * symbol_count);
    for (uint16_t i = 0; i < symbol_count; i++) {
        const uint8_t format = next_byte(file);
        if (format == LM_INTEGER) {
            symbols[i] = make_int(next_int4(file));
        } else if (format == LM_STRING) {
            LM_String *s = malloc(sizeof(LM_String));
            s->length = next_int4(file);
            s->string = next_str(file, s -> length);
            symbols[i] = make_string(s);
        } else if (format == LM_FUNCTION) {
            LM_Function *fn = malloc(sizeof(LM_Function));
            fn->name = next_str(file, next_int4(file));
            if (strcmp(fn->name, INIT_POINT) == 0) {
                program->entry_point = i;
            }
            symbols[i] = make_function(fn);
        } else if (format == LM_BUILT_IN_FUNCTION) {
            LM_Function *fn = malloc(sizeof(LM_Function));
            fn->name = next_str(file, next_int4(file));
            load_native_function(fn);
            symbols[i] = make_function(fn);
        } else if (format == LM_NONE) {
            symbols[i] = make_none();
        } else {
            printf("%d\n", format);
            Fault(INIT_FAULT, "Unhandled DataType");
        }
    }
    return symbols;
}

void read_struct(FILE *file) {
    const uint16_t struct_count = next_int2(file);
    if (struct_count != 0) {
        Fault(INIT_FAULT, "Struct not supported yet");
    }
}

LM_OpCode *read_function_body(FILE *file) {
    const uint32_t body_count = next_int4(file); // lol
    LM_OpCode *body = malloc(sizeof(LM_OpCode) * body_count);
    for (uint32_t i = 0; i < body_count; i++) {
        LM_OpCode *op = malloc(sizeof(LM_OpCode));
        op->op_code = next_byte(file);
        op->value = next_int2(file);
        body[i] = *op;
    }
    return body;
}

void read_functions(FILE *file, const LM_Value *symbol_table) {
    if (symbol_table == NULL) { return; } // fix this later
    const uint16_t function_count = next_int2(file);
    for (uint16_t i = 0; i < function_count; i++) {
        const uint16_t fn_pointer = next_int2(file);
        LM_Function *fn = symbol_table[fn_pointer].as.function;
        fn->args_count = next_byte(file);
        fn->local_count = next_int4(file);
        fn->is_native = false;
        next_int2(file);
        fn->body = read_function_body(file);
    }
}

Program *parse_byte_code(FILE *file) {
    Program *program = malloc(sizeof(Program));
    validate_header(file);
    program->entry_point = -1;
    program->symbol_table = read_symbols(file, program);
    read_struct(file);
    read_functions(file, program->symbol_table);
    return program;
}