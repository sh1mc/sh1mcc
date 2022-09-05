#ifndef SH1MCC_H
#define SH1MCC_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_RESERVED,
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
} NodeKind;

typedef struct node Node;

struct node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Token *tokenize(char *p);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, size_t len);
Token *tokenize(char *p);

Node *expr();

void gen(Node *node);

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

#endif