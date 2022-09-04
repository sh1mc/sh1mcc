#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number.");
        exit(EXIT_FAILURE);
    }

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    char* p = argv[1];

    printf("\tmov rax, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+') {
            p++;
            printf("\tadd rax, %ld\n", strtol(p, &p, 10));
            continue;
        }
        if (*p == '-') {
            p++;
            printf("\tsub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }
        fprintf(stderr, "Unexpected input: '%c'", *p);
        exit(EXIT_FAILURE);
    }
    printf("\tret\n");
    return 0;
}