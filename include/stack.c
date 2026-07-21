#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "error.h"

void raise_stack_underflow();
void raise_stack_overflow();

void stack_push(LM_Stack *stack, const LM_Value value) {
    if (stack->length >= stack->capacity) {
        if (stack->capacity == 0) {
            stack->capacity = STACK_INIT_SIZE;
        } else {
            stack->capacity *= 2;
        }
        LM_Value* new_values = realloc(stack->values, stack->capacity * sizeof(*stack->values));
        if (new_values == NULL) raise_stack_overflow();
        stack->values = new_values;
    }
    stack->values[stack->length++] = value;
}

LM_Value stack_peek_n(const LM_Stack *stack, const int offset) {
    if (offset >= stack->length) raise_stack_underflow();
    return stack->values[stack->length - 1 - offset];
}

LM_Value stack_peek(const LM_Stack *stack) {
    if (stack->length == 0) raise_stack_underflow();
    return stack->values[stack->length - 1];
}

void stack_pop_n(LM_Stack *stack, const int offset) {
    if (offset > stack->length) raise_stack_underflow();
    stack->length -= offset;
}

LM_Value stack_pop(LM_Stack *stack) {
    if (stack->length == 0) raise_stack_underflow();
    return stack->values[--stack->length];
}

void stack_gc(LM_Stack *stack) {
    free(stack->values);
    stack->values = NULL;
    stack->capacity = 0;
    stack->length = 0;
}

void raise_stack_overflow() {
    Fault(STACK_OVERFLOW, "Core Stack Overflow");
}

void raise_stack_underflow() {
    Fault(STACK_UNDERFLOW, "Core Stack Underflow");
}