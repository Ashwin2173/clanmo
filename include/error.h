#ifndef CLANMO_ERROR_H
#define CLANMO_ERROR_H
#include <stdio.h>
#include <stdlib.h>

enum FaultType {
    INIT_FAULT,
    NO_MAIN,
    CORE_FAULT,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    OFF_MEMORY,
    NON_CALLABLE,
    TYPE_ERROR,
    NULL_POINTER_EXCEPTION,
    DIVIDE_BY_ZERO,
    INDEX_OUT_OF_RANGE,
    ATTRIBUTE_ERROR

};

[[noreturn]]
static void Fault(const enum FaultType ft, char *message) {
    fprintf(stderr, "\nFaulted with error code %d:\n%s\n", ft, message);
    exit(EXIT_FAILURE);
}

#endif