#ifndef CLANMO_PROGRAM_H
#define CLANMO_PROGRAM_H

#include "value.h"

typedef struct {
    LM_Value *symbol_table;
    uint16_t symbol_count;
    int32_t entry_point;
} Program;

void program_gc(Program *program);

#endif
