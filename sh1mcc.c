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
    printf("\tmov rax, %d\n", atoi(argv[1]));
    printf("\tret\n");
    return 0;
}