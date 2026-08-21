#include <stdlib.h>

#include "structs.h"

void init_struct(LM_Struct *new, const size_t size) {
    new->length = size;
    new->members = calloc(size, sizeof(LM_Index));
}

size_t hash(const LM_Struct *s, const size_t m_id) {
    return m_id % s->length;
}

void add_member(const LM_Struct *s, const size_t m_id, const size_t value) {
    size_t hash_index = hash(s, m_id);
    // todo: fix this; handle this better as there might a symbol with offset 0
    while (s->members[hash_index].key != 0) {
        hash_index = (hash_index + 1) % s->length;
    }
    s->members[hash_index].key = m_id;
    s->members[hash_index].value = value;
}

size_t get_member(const LM_Struct *s, const size_t m_id) {
    size_t hash_index = hash(s, m_id);
    while (s->members[hash_index].key != m_id) {
        hash_index = (hash_index + 1) % s->length;
    }
    return s->members[hash_index].value;
}
