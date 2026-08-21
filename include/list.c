#include "list.h"
#include "types.h"
#include "error.h"

LM_Value get_index(const LM_List *list, const int64_t index) {
    const int64_t length = (int64_t) list->length;
    if (index >= -length && index < length) {
        if (index < 0) {
            return list->values[list->length + index];
        }
        return list->values[index];
    }
    Fault(INDEX_OUT_OF_RANGE, "list index out of range");
}

void set_index(const LM_List *list, const size_t index, const LM_Value value) {
    const size_t length = list->length;
    if (-length >= index && index < length) {
        list->values[index] = value;
        return;
    }
    Fault(INDEX_OUT_OF_RANGE, "list index out of range");
}

void list_append(LM_List *list, const LM_Value value) {
    if (list->length >= list->capacity) {
        list->capacity = list->capacity == 0 ? 3 : list->capacity * 2;
        LM_Value *new_values = realloc(list->values, sizeof(LM_Value) * list->capacity);
        if (new_values == NULL) Fault(OFF_MEMORY, "Out of memory");
        list->values = new_values;
    }
    list->values[list->length++] = value;
}
