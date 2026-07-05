#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "vm.h"

#define STACK_SIZE 1024
#define FRAME_SIZE 2000

void load_main(VM *vm);
Value pop(VM* vm);
void push(VM *vm, const Value *value);
void check_underflow(const VM *vm, int count);
void check_overflow(const VM *vm, int count);

void op_push(VM *vm, OpCode opcode);
void op_pop(VM *vm, OpCode opcode);
void op_call(VM *vm, OpCode opcode);
void op_ret (VM *vm, OpCode opcode);

void init_vm(VM *vm) {
    vm->stack = malloc(sizeof(Value) * STACK_SIZE);
    vm->stack_pointer = 0;
    vm->frames = malloc(sizeof(Value) * FRAME_SIZE);
    vm->frame_pointer = 0;
    load_main(vm);
}

void vm_run(FILE *file) {
    VM *vm = load_byte_code(file);
    init_vm(vm);
    while (vm->frame_pointer != 0) {
        Frame *frame = &vm->frames[vm->frame_pointer-1];
        const OpCode opcode = frame->function->body[frame->inst_pointer++];
        switch (opcode.op_code) {
            case OP_PUSH: op_push(vm, opcode); break;
            case OP_POP:  op_pop(vm, opcode); break;
            case OP_CALL: op_call(vm, opcode); break;
            case OP_RET : op_ret(vm, opcode); break;
            default:
                printf("%d", opcode.op_code);
                Fault(CORE_FAULT, "Unhandled OpCode");
            break;
        }
    }
}

void load_function(VM *vm, const Value *value) {
    Frame *frame = malloc(sizeof(Frame));
    frame->function = value->as.function;
    frame->inst_pointer = 0;
    frame->base_pointer = vm->stack_pointer;
    vm->frames[vm->frame_pointer++] = *frame;
}

void load_main(VM *vm) {
    if (vm->main_fn_pointer == -1) {
        Fault(NO_MAIN, "No 'main' function defined");
    }
    load_function(vm, &vm->symbol_table[vm->main_fn_pointer]);
}

void op_push(VM *vm, const OpCode opcode) {
    push(vm, &vm->symbol_table[opcode.value]);
}

void op_pop(VM *vm, const OpCode opcode) {
    pop(vm);
}

void op_call(VM *vm, const OpCode opcode) {
    check_underflow(vm, opcode.value-1);
    const Function fn = *vm->stack[vm->stack_pointer - opcode.value - 1].as.function;
    if (fn.is_native) {
        Value *args = &vm->stack[vm->stack_pointer - opcode.value];
        const Value value = fn.native_function(opcode.value, args);
        vm->stack_pointer -= opcode.value + 1;
        push(vm, &value);
        return;
    }
    load_function(vm, &vm->symbol_table[opcode.value-1]);
}

void op_ret(VM *vm, const OpCode opcode){
    const Value value = pop(vm);
    vm->stack_pointer -= vm->frames[--vm->frame_pointer].base_pointer-1;
    push(vm, &value);
}

Value pop(VM* vm) {
    check_underflow(vm, 1);
    const Value *value = &vm->stack[vm->stack_pointer - 1];
    vm->stack_pointer--;
    return *value;
}

void push(VM *vm, const Value *value) {
    check_overflow(vm, 1);
    vm->stack[vm->stack_pointer++] = *value;
}

void check_underflow(const VM *vm, const int count) {
    if (vm->stack_pointer - count < vm->frames[vm->frame_pointer-1].base_pointer) {
        Fault(STACK_UNDERFLOW, "Stack underflow");
    }
}

void check_overflow(const VM *vm, const int count) {
    if (vm->stack_pointer + count >= STACK_SIZE) {
        Fault(STACK_OVERFLOW, "Stack overflow");
    }
}