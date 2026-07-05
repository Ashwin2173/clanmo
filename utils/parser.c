#include <stdio.h>
#include <string.h>

#include "vm.h"
#include "parser.h"
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

void check_native(Function *function) {
    const char *fn_name = function->name;
    if (strcmp(fn_name, "print") == 0) {
        function->is_native = true;
        function->native_function = native_print;
    } else if (strcmp(fn_name, "input") == 0) {
        function->is_native = true;
        function->native_function = native_input;
    }
}

Value *read_symbols(FILE *file, VM *vm) {
    const uint16_t symbol_count = next_int2(file);
    Value *symbols = malloc(sizeof(Value) * symbol_count);
    for (uint16_t i = 0; i < symbol_count; i++) {
        const uint8_t format = next_byte(file);
        if (format == LM_INTEGER) {
            symbols[i] = make_int(next_int4(file));
        } else if (format == LM_STRING) {
            String *s = malloc(sizeof(String));
            s->length = next_int4(file);
            s->string = next_str(file, s -> length);
            symbols[i] = make_string(s);
        } else if (format == LM_FUNCTION) {
            Function *fn = malloc(sizeof(Function));
            fn->name = next_str(file, next_int4(file));
            if (strcmp(fn->name, INIT_POINT) == 0) {
                vm->main_fn_pointer = i;
            }
            check_native(fn);
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

OpCode *read_function_body(FILE *file) {
    const uint32_t body_count = next_int4(file); // lol
    OpCode *body = malloc(sizeof(OpCode) * body_count);
    for (uint32_t i = 0; i < body_count; i++) {
        OpCode *op = malloc(sizeof(OpCode));
        op->op_code = next_byte(file);
        op->value = next_int2(file);
        body[i] = *op;
    }
    return body;
}

void read_functions(FILE *file, const Value *symbol_table) {
    if (symbol_table == NULL) { return; } // fix this later
    const uint16_t function_count = next_int2(file);
    for (uint16_t i = 0; i < function_count; i++) {
        const uint16_t fn_pointer = next_int2(file);
        Function *fn = symbol_table[fn_pointer].as.function;
        fn->args_count = next_byte(file);
        fn->local_count = next_int4(file);
        next_int2(file);
        fn->body = read_function_body(file);
    }
}

VM *load_byte_code(FILE *file) {
    VM *vm = malloc(sizeof(VM));
    validate_header(file);
    vm->main_fn_pointer = -1;
    vm->symbol_table = read_symbols(file, vm);
    read_struct(file);
    read_functions(file, vm->symbol_table);
    return vm;
}
