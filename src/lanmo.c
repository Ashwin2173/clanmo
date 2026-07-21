#include <stdio.h>
#include <stdlib.h>

#include "../include/vm.h"
#include "../include/value.h"
#include "../include/parser.h"
#include "../include/program.h"

void op_call(const LM_VM *vm, LM_OpCode opcode);
void op_ret (const LM_VM *vm, LM_OpCode opcode);

void load_main(const LM_VM *vm, const Program *program);
void load_function(const LM_VM *vm, const LM_Value *value);

void init_vm(LM_VM *vm, const Program *program) {
    vm->stack = malloc(sizeof(LM_Stack));
    vm->stack->values = NULL;
    vm->stack->length = 0;
    vm->stack->capacity = 0;
    vm->frames = malloc(sizeof(LM_Frame));
    vm->frames->values = NULL;
    vm->frames->length = 0;
    vm->frames->capacity = 0;
    load_main(vm, program);
}

void vm_run(FILE *file) {
    const Program *program = parse_byte_code(file);
    LM_VM *vm = malloc(sizeof(LM_VM));
    init_vm(vm, program);
    while (vm->frames->length != 0) {
        LM_Frame *frame = frame_peek(vm->frames);
        const LM_OpCode inst = frame->function->body[frame->inst_ptr++];
        switch (inst.op_code) {
            case OP_PUSH:
                stack_push(vm->stack, program->symbol_table[inst.value]);
                break;
            case OP_POP:
                stack_pop(vm->stack);
                break;
            case OP_CALL:
                op_call(vm, inst);
                break;
            case OP_RET :
                op_ret(vm, inst);
                break;
            default:
                printf("%d", inst.op_code);
                Fault(CORE_FAULT, "Unhandled OpCode");
            break;
        }
    }
}

void load_function(const LM_VM *vm, const LM_Value *value) {
    LM_Frame *frame = malloc(sizeof(LM_Frame));
    frame->function = value->as.function;
    frame->inst_ptr = 0;
    frame->base_ptr = vm->stack->length;
    frame_push(vm->frames, *frame);
}

void load_main(const LM_VM *vm, const Program *program) {
    if (program->entry_point == -1) {
        Fault(NO_MAIN, "No 'main' function defined");
    }
    load_function(vm, &program->symbol_table[program->entry_point]);
}

void op_call(const LM_VM *vm, const LM_OpCode opcode) {
    const LM_Function* fn = stack_peek_n(vm->stack, opcode.value).as.function;
    if (fn->is_native) {
        LM_Value args = vm->stack->values[vm->stack->length - opcode.value];
        const LM_Value value = fn->native_function(opcode.value, &args);
        stack_pop_n(vm->stack, opcode.value - 1);
        stack_push(vm->stack, value);
        return;
    }
    load_function(vm, &vm->symbol_table[opcode.value-1]);
}

void op_ret(const LM_VM *vm, const LM_OpCode opcode){
    const LM_Value value = stack_pop(vm->stack);
    stack_pop_n(vm->stack, frame_pop(vm->frames).base_ptr-1);
    stack_push(vm->stack, value);
}
