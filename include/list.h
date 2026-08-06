#ifndef CLANMO_LIST_H
#define CLANMO_LIST_H

#include "types.h"

typedef struct LM_List {
    LM_Value *values;
    size_t length;
    size_t capacity;
} LM_List;

// void list_append(LM_List *list, LM_Value value);
// void list_gc(LM_List *list);

#endif
