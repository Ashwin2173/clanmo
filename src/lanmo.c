#include <stdio.h>
#include <stdlib.h>

#include "../include/vm.h"
#include "../include/value.h"
#include "../include/parser.h"
#include "../include/program.h"

void op_call(LM_VM *vm, LM_OpCode opcode);
void op_ret (LM_VM *vm, LM_OpCode opcode);

void load_main(LM_VM *vm, const Program *program);
void load_function(LM_VM *vm, int args);

void init_vm(LM_VM *vm, const Program *program) {
    vm->stack.values = NULL;
    vm->stack.length = 0;
    vm->stack.capacity = 0;
    vm->frames.values = NULL;
    vm->frames.length = 0;
    vm->frames.capacity = 0;
    load_main(vm, program);
}

void vm_run(FILE *file) {
    const Program *program = parse_byte_code(file);
    LM_VM vm;
    init_vm(&vm, program);
    while (vm.frames.length != 0) {
        LM_Frame *frame = frame_peek(&vm.frames);
        const LM_OpCode inst = frame->function->body[frame->inst_ptr++];
        switch (inst.op_code) {
            case OP_PUSH:
                stack_push(&vm.stack, program->symbol_table[inst.value]);
                break;
            case OP_POP:
                stack_pop(&vm.stack);
                break;
            case OP_CALL:
                op_call(&vm, inst);
                break;
            case OP_RET :
                op_ret(&vm, inst);
                break;
            default:
                printf("%d", inst.op_code);
                Fault(CORE_FAULT, "Unhandled OpCode");
            break;
        }
    }
}

void load_function(LM_VM *vm, const int args) {
    const LM_Value value = stack_peek_n(&vm->stack, vm->stack.length - args - 1);
    LM_Frame frame;
    frame.function = value.as.function;
    frame.inst_ptr = 0;
    frame.base_ptr = vm->stack.length - args - 1;
    frame_push(&vm->frames, frame);
}

void load_main(LM_VM *vm, const Program *program) {
    if (program->entry_point == -1) {
        Fault(NO_MAIN, "No 'main' function defined");
    }
    stack_push(&vm->stack, program->symbol_table[program->entry_point]);
    load_function(vm, 0);
}

void op_call(LM_VM *vm, const LM_OpCode opcode) {
    const LM_Function* fn = stack_peek_n(&vm->stack, opcode.value).as.function;
    if (fn->is_native) {
        LM_Value *args = &vm->stack.values[vm->stack.length - opcode.value];
        const LM_Value value = fn->native_function(opcode.value, args);
        stack_pop_n(&vm->stack, opcode.value + 1);
        stack_push(&vm->stack, value);
        return;
    }
    load_function(vm, opcode.value);
}

void op_ret(LM_VM *vm, const LM_OpCode opcode){
    const LM_Value value = stack_pop(&vm->stack);
    stack_pop_n(&vm->stack, frame_pop(&vm->frames).base_ptr);
    stack_push(&vm->stack, value);
}
