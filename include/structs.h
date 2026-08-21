#ifndef CLANMO_STRUCTS_H
#define CLANMO_STRUCTS_H

#include "types.h"

typedef struct {
    size_t key;
    size_t value;
} LM_Index;

typedef struct {
    size_t length;
    LM_Index *members;
} LM_Struct;

typedef struct LM_Object {
    LM_Struct *definition;
    LM_Value *data;
} LM_Object;

void init_struct(LM_Struct *new, size_t size);
void add_member(const LM_Struct *s, size_t m_id, size_t value);
size_t get_member(const LM_Struct *s, size_t m_id);

#endif
