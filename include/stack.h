#ifndef CLANMO_STACK_H
#define CLANMO_STACK_H

#include "value.h"

#define STACK_INIT_SIZE 1024

typedef struct {
    LM_Value *values;
    size_t length;
    size_t capacity;
} LM_Stack;

void stack_init(LM_Stack *stack);
void stack_push(LM_Stack *stack, LM_Value value);
LM_Value stack_peek_n(const LM_Stack *stack, size_t offset);
LM_Value stack_peek(const LM_Stack *stack);
void stack_pop_n(LM_Stack *stack, size_t count);
LM_Value stack_pop(LM_Stack *stack);
void stack_gc(LM_Stack *stack);

#endif
