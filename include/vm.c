#include "vm.h"
#include "error.h"

void raise_frame_underflow();

void frame_init(LM_FrameStack *frame) {
    frame->values = NULL;
    frame->length = 0;
    frame->capacity = 0;
}

void frame_push(LM_FrameStack *stack, const LM_Frame value) {
    if (stack->length >= stack->capacity) {
        if (stack->capacity == 0) {
            stack->capacity = STACK_INIT_SIZE;
        } else {
            stack->capacity *= 2;
        }
        LM_Frame* new_values = realloc(stack->values, stack->capacity * sizeof(*stack->values));
        if (new_values == NULL) {
            Fault(STACK_OVERFLOW, "Core Stack Overflow");
        }
        stack->values = new_values;
    }
    stack->values[stack->length++] = value;
}

LM_Frame *frame_peek(const LM_FrameStack *stack) {
    if (stack->length == 0) raise_frame_underflow();
    return &stack->values[stack->length - 1];
}

LM_Frame frame_pop(LM_FrameStack *stack) {
    if (stack->length == 0) raise_frame_underflow();
    return stack->values[--stack->length];
}

void frames_gc(LM_FrameStack *stack) {
    free(stack->values);
    stack->values = NULL;
    stack->capacity = 0;
    stack->length = 0;
}

void raise_frame_underflow() {
    Fault(STACK_UNDERFLOW, "Frame Stack Underflow");
}