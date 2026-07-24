#ifndef CLANMO_NATIVE_FUNCTION_H
#define CLANMO_NATIVE_FUNCTION_H

#include "../include/value.h"

LM_Value native_print(size_t argc, LM_Value* args);
LM_Value native_input(size_t argc, LM_Value* args);
LM_Value native_now(size_t argc, LM_Value* args);

#endif
