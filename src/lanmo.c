#include <stdio.h>
#include <stdlib.h>

#include "../include/arithmetic.h"
#include "../include/vm.h"
#include "../include/value.h"
#include "../include/parser.h"
#include "../include/program.h"

inline void op_ret (LM_VM *vm);
inline void op_bin(LM_VM *vm, LM_OpCode op_code);
inline void op_call(LM_VM *vm, LM_OpCode opcode);
inline void op_store(LM_VM *vm, const LM_Frame *frame, LM_OpCode op_code);
inline void op_load(LM_VM *vm, const LM_Frame *frame, LM_OpCode op_code);
inline void op_jump(LM_Frame *frame, LM_OpCode op_code);
inline void op_jump_if_false(LM_VM *vm, LM_Frame *frame, LM_OpCode op_code);

void load_main(LM_VM *vm, const Program *program);
void call_function(LM_VM *vm, size_t args);

void init_vm(LM_VM *vm, const Program *program) {
    stack_init(&vm->stack);
    frame_init(&vm->frames);
    memory_init(&vm->memory);
    load_main(vm, program);
}

void vm_run(FILE *file) {
    Program *program = parse_byte_code(file);
    LM_VM vm;
    init_vm(&vm, program);
    while (vm.frames.length != 0) {
        LM_Frame *frame = frame_peek(&vm.frames);
        const LM_OpCode inst = frame->function->body[frame->inst_ptr++];
        switch (inst.op_code) {
            case OP_PUSH:
                stack_push(&vm.stack, program->symbol_table[inst.value]);
                break;
            case OP_BIN: op_bin(&vm, inst); break;
            case OP_POP: stack_pop(&vm.stack); break;
            case OP_STORE: op_store(&vm, frame, inst); break;
            case OP_LOAD: op_load(&vm, frame, inst); break;
            case OP_CALL: op_call(&vm, inst); break;
            case OP_RET : op_ret(&vm); break;
            case OP_JUMP: op_jump(frame, inst); break;
            case OP_JUMP_IF_FALSE: op_jump_if_false(&vm, frame, inst); break;
            default:
                printf("%d", inst.op_code);
                Fault(CORE_FAULT, "Unhandled OpCode");
        }
    }
    stack_gc(&vm.stack);
    frames_gc(&vm.frames);
    program_gc(program);
}

void call_function(LM_VM *vm, const size_t args) {
    const LM_Value value = vm->stack.values[vm->stack.length - args - 1];
    LM_Frame frame;
    frame.function = value.as.function;
    frame.inst_ptr = 0;
    frame.base_ptr = vm->stack.length - args - 1;
    frame.mem_ptr = vm->memory.length;
    frame_push(&vm->frames, frame);
}

void load_main(LM_VM *vm, const Program *program) {
    if (program->entry_point == -1) {
        Fault(NO_MAIN, "No 'main' function defined");
    }
    stack_push(&vm->stack, program->symbol_table[program->entry_point]);
    call_function(vm, 0);
}

void op_bin(LM_VM *vm, const LM_OpCode op_code) {
    if (vm->stack.length < 2) {
        Fault(STACK_UNDERFLOW, "Stack overflow");
    }
    LM_Value *left = &vm->stack.values[vm->stack.length - 2];
    const LM_Value *right = &vm->stack.values[vm->stack.length - 1];
    if (op_code.value > 0 && op_code.value > OP_COUNT) {
        Fault(CORE_FAULT, "invalid bin op value");
    }
    binop_dispatch_table[op_code.value - 1](left, right);
    vm->stack.length--;
}

void op_jump(LM_Frame *frame, const LM_OpCode op_code) {
    frame->inst_ptr = op_code.value;
}

void op_jump_if_false(LM_VM *vm, LM_Frame *frame, const LM_OpCode op_code) {
    const LM_Value value = stack_pop(&vm->stack);
    if (value.type != LM_BOOLEAN) Fault(TYPE_ERROR, "Required boolean for test");
    if (!value.as.boolean) {
        frame->inst_ptr = op_code.value;
    }
}

void op_store(LM_VM *vm, const LM_Frame *frame, const LM_OpCode op_code) {
    const LM_Value value = stack_peek(&vm->stack);
    const size_t location = frame->mem_ptr + op_code.value;
    memory_write(&vm->memory, location, value);
}

void op_load(LM_VM *vm, const LM_Frame *frame, const LM_OpCode op_code) {
    const size_t location = frame->mem_ptr + op_code.value;
    const LM_Value value = memory_read(&vm->memory, location);
    stack_push(&vm->stack, value);
}

void op_call(LM_VM *vm, const LM_OpCode opcode) {
    const LM_Value value = stack_peek_n(&vm->stack, opcode.value);
    if (value.type != LM_FUNCTION) Fault(NON_CALLABLE, "Invoking non callable");
    const LM_Function *fn = value.as.function;
    if (fn->is_native) {
        LM_Value *args = &vm->stack.values[vm->stack.length - opcode.value];
        const LM_Value ret_value = fn->native_function(opcode.value, args);
        stack_pop_n(&vm->stack, opcode.value + 1);
        stack_push(&vm->stack, ret_value);
        return;
    }
    call_function(vm, opcode.value);
}

void op_ret(LM_VM *vm){
    const LM_Value value = stack_pop(&vm->stack);
    const LM_Frame frame = frame_pop(&vm->frames);
    vm->stack.length = frame.base_ptr;
    vm->memory.length = frame.mem_ptr;
    stack_push(&vm->stack, value);
}
