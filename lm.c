#include <stdio.h>
#include <stdlib.h>

#include "utils/vm.h"

void print_usage() {
    printf("USAGE:\n");
    printf("    lm <lmc_file_path>.lmc\n\n");
    exit(EXIT_FAILURE);
}

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        print_usage();
    }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("[ERROR] Could not open file '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    vm_run(file);
    fclose(file);
    return 0;
}
