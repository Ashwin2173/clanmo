#include <stdio.h>
#include <string.h>

#include "../include/vm.h"
#include "../include/parser.h"
#include "native_functions.h"

#define MAGIC 2273
#define MAJOR 1
#define MINOR 0

static const char INIT_POINT[] = "main";

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
    } else if (strcmp(fn_name, "now") == 0) {
        function->is_native = true;
        function->native_function = native_now;
    } else if (strcmp(fn_name, "len") == 0) {
        function->is_native = true;
        function->native_function = native_len;
    }else {
        Fault(INIT_FAULT, "Unknown built-in function");
    }
}

LM_Value *parse_symbols(FILE *file, Program *program) {
    const uint16_t symbol_count = next_int2(file);
    LM_Value *symbols = malloc(sizeof(LM_Value) * symbol_count);
    program->symbol_count = symbol_count;
    for (uint16_t i = 0; i < symbol_count; i++) {
        const uint8_t format = next_byte(file);
        if (format == LM_INTEGER) {
            const uint32_t size = next_int4(file);
            if (size != 4) Fault(INIT_FAULT, "Invalid int size");
            symbols[i] = make_int(next_int4(file));
        } else if (format == LM_STRING) {
            LM_String *s = malloc(sizeof(*s));
            s->length = next_int4(file);
            s->string = next_str(file, s->length);
            symbols[i] = make_string(s);
        } else if (format == LM_MEMBER) {
            LM_Member *member = malloc(sizeof(*member));
            member->name = next_str(file, next_int4(file));
            symbols[i] = make_member(member);
        } else if (format == LM_FUNCTION) {
            LM_Function *fn = malloc(sizeof(*fn));
            fn->name = next_str(file, next_int4(file));
            if (strcmp(fn->name, INIT_POINT) == 0) {
                program->entry_point = i;
            }
            symbols[i] = make_function(fn);
        } else if (format == LM_BUILT_IN_FUNCTION) {
            LM_Function *fn = malloc(sizeof(*fn));
            fn->name = next_str(file, next_int4(file));
            fn->body = NULL;
            load_native_function(fn);
            symbols[i] = make_function(fn);
        } else if (format == LM_BOOLEAN) {
            const bool value = next_byte(file);
            symbols[i] = make_boolean(value);
        } else if (format == LM_NONE) {
            symbols[i] = make_none();
        } else {
            printf("%d\n", format);
            Fault(INIT_FAULT, "Unhandled DataType");
        }
    }
    return symbols;
}

LM_Struct *parse_struct(FILE *file) {
    const uint16_t struct_count = next_int2(file);
    LM_Struct *s = malloc(sizeof(*s) * struct_count);
    for (size_t i = 0; i < struct_count; i++) {
        const size_t m_size = next_byte(file);
        init_struct(&s[0], m_size);
        for (size_t j = 0; j < m_size; j++) {
            const size_t offset = next_int2(file);
            add_member(&s[0], offset, j);
        }
    }
    return s;
}

LM_OpCode *parse_function_body(FILE *file) {
    const uint32_t body_count = next_int4(file); // lol
    LM_OpCode *body = malloc(sizeof(LM_OpCode) * body_count);
    for (uint32_t i = 0; i < body_count; i++) {
        body[i].op_code = next_byte(file);
        body[i].value = next_int2(file);
    }
    return body;
}

void parse_functions(FILE *file, const LM_Value *symbol_table) {
    if (symbol_table == NULL) Fault(INIT_FAULT, "Symbol table missing");
    const uint16_t function_count = next_int2(file);
    for (uint16_t i = 0; i < function_count; i++) {
        const uint16_t fn_pointer = next_int2(file);
        LM_Function *fn = symbol_table[fn_pointer].as.function;
        fn->args_count = next_byte(file);
        fn->local_count = next_int4(file);
        fn->is_native = false;
        next_int2(file);
        fn->body = parse_function_body(file);
    }
}

Program *parse_byte_code(FILE *file) {
    Program *program = malloc(sizeof(Program));
    validate_header(file);
    program->entry_point = -1;
    program->symbol_table = parse_symbols(file, program);
    program->struct_lookup = parse_struct(file);
    parse_functions(file, program->symbol_table);
    return program;
}