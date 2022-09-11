#ifndef SH1MCC_H
#define SH1MCC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct token Token;

struct token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    size_t len;
};

extern Token *token;
extern char *user_input;

typedef enum {
    ND_LESS,
    ND_LESS_EQ,
    ND_EQ,
    ND_NEQ,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_LVAR,
    ND_ASSIGN,
} NodeKind;

typedef struct node Node;

struct node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    // kind == ND_NUM
    int val;
    // kind == ND_LVAR
    int offset;
};

#define MAX_PROGRAM_LEN 100
extern Node *code[MAX_PROGRAM_LEN];

Token *tokenize(char *p);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
bool at_num();
Token *new_token(TokenKind kind, Token *cur, char *str, size_t len);
Token *tokenize(char *p);

void program();

void gen(Node *node);

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

#endif