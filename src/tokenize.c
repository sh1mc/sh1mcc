#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "sh1mcc.h"

bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        error_at(token->str, "Unexpected token: %c", op);
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "The token is not number.");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() { return token->kind == TK_EOF; }

Token *new_token(TokenKind kind, Token *cur, char *str, size_t len) {
    Token *next = malloc(sizeof(Token));
    next->kind = kind;
    next->str = str;
    next->len = len;
    next->next = NULL;
    cur->next = next;
    return next;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (strncmp(p, "==", 2) == 0 ||
            strncmp(p, "!=", 2) == 0 ||
            strncmp(p, ">=", 2) == 0 ||
            strncmp(p, "<=", 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' ||
            *p == '(' || *p == ')' || *p == '<' || *p == '>') {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p += 1;
            continue;
        }
        if (isdigit(*p)) {
            int old_p = (long)p;
            int val = strtol(p, &p, 10);
            unsigned digit_len = (long)p - old_p;
            cur = new_token(TK_NUM, cur, p, digit_len);
            cur->val = val;
            continue;
        }

        error_at(p, "Tokenize failed.");
    }
    new_token(TK_EOF, cur, p, 1);
    return head.next;
}