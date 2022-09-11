#include <stdio.h>

#include "sh1mcc.h"

void free_token(Token *t) {
    if (t->next) {
        free_token(t->next);
    }
    free(t);
}

void free_ast(Node *n) {
    if (n->lhs) {
        free_ast(n->lhs);
    }
    if (n->rhs) {
        free_ast(n->rhs);
    }
    free(n);
}

Token *token;
char *user_input;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number.");
        exit(EXIT_FAILURE);
    }

    user_input = argv[1];
    token = tokenize(user_input);
    // Parse result will be stored in *code.
    program();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // Reserve memory for 26 variables.
    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, 208\n");

    // Generate
    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        printf("\tpop rax;\n");
    }

    // The return value is rax.
    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");

    free_token(token);
    for (int i = 0; code[i]; i++) {
        free_ast(code[i]);
    }
    return 0;
}