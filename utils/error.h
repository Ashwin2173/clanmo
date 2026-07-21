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
};

static void Fault(const enum FaultType ft, char *message) {
    fprintf(stderr, "Faulted with error code %d:\n%s\n", ft, message);
    exit(EXIT_FAILURE);
}

#endif