#ifndef CLANMO_MEMORY_H
#define CLANMO_MEMORY_H

#include "value.h"

#define INIT_MEMORY_SIZE 1024

typedef struct {
    LM_Value *values;
    size_t length;
    size_t capacity;
} LM_Memory;

void memory_init(LM_Memory *memory);
void memory_write(LM_Memory *memory, size_t location, LM_Value value);
LM_Value memory_read(const LM_Memory *memory, size_t location);
void memory_free(LM_Memory *memory);

#endif
