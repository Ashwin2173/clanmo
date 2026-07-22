#ifndef CLANMO_VM_H
#define CLANMO_VM_H

#include "value.h"
#include "stack.h"

typedef struct {
    LM_Function *function;
    uint64_t inst_ptr;
    uint64_t base_ptr;
} LM_Frame;

typedef struct {
    LM_Frame *values;
    size_t length;
    size_t capacity;
} LM_FrameStack;

typedef struct {
    LM_Stack stack;
    LM_FrameStack frames;
} LM_VM;

void frame_push(LM_FrameStack *stack, LM_Frame value);
LM_Frame *frame_peek(const LM_FrameStack *stack);
LM_Frame frame_pop(LM_FrameStack *stack);
void frames_gc(LM_FrameStack *stack);

#endif
