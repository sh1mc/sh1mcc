#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct token Token;

struct token {
    TokenKind kind;
    Token* next;
    int val;
    char* str;
};

Token* token;

void error(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

bool consume(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }
    token = token->next;
    return true;
}

void expect(char op)
{
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        error("The token is not %c.", op);
    }
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM) {
        error("The token is not number.");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

Token* new_token(TokenKind kind, Token* cur, char* str)
{
    Token* next = malloc(sizeof(Token));
    next->kind = kind;
    next->str = str;
    next->next = NULL;
    cur->next = next;
    return next;
}

Token* tokenize(char* p)
{
    Token head;
    head.next = NULL;
    Token* cur = &head;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p);
            p++;
            continue;
        }
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("Tokenize failed.");
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number.");
        exit(EXIT_FAILURE);
    }

    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // The first token must be a number
    printf("\tmov rax, %d\n", expect_number());

    while (!at_eof()) {
        if (consume('+'))
        {
            printf("\tadd rax, %d\n", expect_number());
            continue;
        }
        expect('-');
        printf("\tsub rax, %d\n", expect_number());
    }
    printf("\tret\n");
    return 0;
}