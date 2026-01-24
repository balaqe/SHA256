// EXAMPLE USAGE OF LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "sha256.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Unexpected number of argument. Usage: %s [pass]\n", argv[0]);
        return 1;
    }

    printf("Your input was %s\n", argv[1]);

    char *res = malloc (257);
    res = sha256(argv[1], res);

    printf("%s", res);

    free(res);
    res = NULL;

    return 0;
}
