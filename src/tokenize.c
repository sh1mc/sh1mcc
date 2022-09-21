#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "sh1mcc.h"

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
        if (strncmp(p, "while", 5) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 5);
            p += 5;
            continue;
        }
        if (strncmp(p, "else", 4) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 4);
            p += 4;
            continue;
        }
        if (strncmp(p, "for", 3) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 3);
            p += 3;
            continue;
        }
        if (strncmp(p, "if", 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
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
            *p == '(' || *p == ')' || *p == '<' || *p == '>' ||
            *p == '=' || *p == ';') {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p += 1;
            continue;
        }

        unsigned ident_len = 0;
        while ('a' <= *(p + ident_len) && *(p + ident_len) <= 'z') {
            ident_len++;
        }
        if (ident_len > 0) {
            cur = new_token(TK_IDENT, cur, p, ident_len);
            p += ident_len;
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