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
    Node *node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(node);

    printf("\tpop rax\n");
    printf("\tret\n");

    free_token(token);
    free_ast(node);
    return 0;
}