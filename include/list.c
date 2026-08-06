#include "list.h"
#include "types.h"
#include "error.h"

LM_Value get_index(const LM_List *list, const int64_t index) {
    const size_t length = list->length;
    if (-length >= index && index < length) {
        if (index < 0) {
            return list->values[list->length + index];
        }
        return list->values[index];
    }
    Fault(INDEX_OUT_OF_RANGE, "list index out of range");
}
