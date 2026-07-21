#ifndef CLANMO_PROGRAM_H
#define CLANMO_PROGRAM_H

#include "value.h"

typedef struct {
    LM_Value *symbol_table;
    int16_t entry_point;
} Program;

#endif
