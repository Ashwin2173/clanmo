#ifndef CLANMO_PARSER_H
#define CLANMO_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "program.h"

Program *parse_byte_code(FILE *file);

static inline void next_bytes(FILE *fp, void *buffer, const size_t size) {
    if (fread(buffer, 1, size, fp) != size) {
        Fault(INIT_FAULT, "Unexpected end of file");
    }
}

static uint8_t next_byte(FILE *fp) {
    char value;
    next_bytes(fp, &value, sizeof(char));
    return value;
}

static uint16_t next_int2(FILE *fp) {
    uint16_t value = 0;
    next_bytes(fp, &value, sizeof(uint16_t));
    return value;
}

static uint32_t next_int4(FILE *fp) {
    long value = 0;
    next_bytes(fp, &value, sizeof(uint32_t));
    return value;
}

static double next_float(FILE *fp) {
    double value;
    if (fread(&value, sizeof(double), 1, fp) != 1) {
        Fault(INIT_FAULT, "Unexpected end of file");
    }
    return value;
}

static char *next_str(FILE *fp, const size_t size) {
    char *str = malloc(size + 1);
    if (!str) {
        Fault(INIT_FAULT, "Out of memory");
    }
    next_bytes(fp, str, size);
    if (str != NULL) str[size] = '\0';
    return str;
}

#endif
