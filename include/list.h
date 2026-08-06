#ifndef CLANMO_LIST_H
#define CLANMO_LIST_H

#include "types.h"

typedef struct LM_List {
    LM_Value *values;
    size_t length;
    size_t capacity;
} LM_List;

LM_Value get_index(const LM_List *list, int64_t index);
// void set_index(LM_List *list, size_t index, LM_Value value);
// void list_append(LM_List *list, LM_Value value);
// void list_gc(LM_List *list);

#endif
