#include "value.h"
#include "error.h"
#include "memory.h"

void memory_init(LM_Memory *memory) {
    memory->values = NULL;
    memory->length = 0;
    memory->capacity = 0;
}

void memory_write(LM_Memory *memory, const size_t location, const LM_Value value) {
    if (memory->length >= memory->capacity) {
        if (memory->capacity == 0) {
            memory->capacity = INIT_MEMORY_SIZE;
        } else {
            memory->capacity *= 2;
        }
        LM_Value* new_values = realloc(memory->values, memory->capacity * sizeof(*memory->values));
        memory->values = new_values;
    }
    memory->values[location] = value;
    memory->length++;
}

LM_Value memory_read(const LM_Memory *memory, const size_t location) {
    if (location >= memory->length) {
        Fault(OFF_MEMORY, "slot out of range");
    }
    return memory->values[location];
}

void memory_free(LM_Memory *memory) {
    free(memory->values);
    memory->values = NULL;
    memory->length = 0;
    memory->capacity = 0;
}
