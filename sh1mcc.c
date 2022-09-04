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

char* user_input;

void error_at(char* loc, char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
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
        error_at(token->str, "Unexpected token: %c", op);
    }
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUM) {
        error_at(token->str, "The token is not number.");
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
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            cur = new_token(TK_RESERVED, cur, p);
            p++;
            continue;
        }
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_at(p, "Tokenize failed.");
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
} NodeKind;

typedef struct node Node;

struct node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
};

Node* new_node(NodeKind kind, Node* lhs, Node* rhs)
{
    Node* node = malloc(sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node* new_node_num(int val)
{
    Node* node = malloc(sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node* expr();
Node* mul();
Node* primary();

Node* expr()
{
    Node* node = mul();
    while (1) {
        if (consume('+')) {
            node = new_node(ND_ADD, node, mul());
        }
        else if (consume('-')) {
            node = new_node(ND_SUB, node, mul());
        }
        else {
            return node;
        }
    }
}

Node* mul()
{
    Node* node = primary();
    while (1) {
        if (consume('*')) {
            node = new_node(ND_MUL, node, primary());
        }
        else if (consume('/')) {
            node = new_node(ND_DIV, node, primary());
        }
        else {
            return node;
        }
    }
}

Node* primary()
{
    if (consume('(')) {
        Node* node = expr();
        expect(')');
        return node;
    }
    return new_node_num(expect_number());
}

void gen(Node* node)
{
    if (node->kind == ND_NUM) {
        printf("\tpush %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("\tadd rax, rdi\n");
        break;
    case ND_SUB:
        printf("\tsub rax, rdi\n");
        break;
    case ND_MUL:
        printf("\timul rax, rdi\n");
        break;
    case ND_DIV:
        printf("\tcqo\n");
        printf("\tidiv rdi\n");
        break;
    default:
        error("Parse failed (unknown rule).");
    }
    printf("\tpush rax\n");
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Invalid argument number.");
        exit(EXIT_FAILURE);
    }

    user_input = argv[1];
    token = tokenize(user_input);
    Node* node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(node);

    printf("\tpop rax\n");
    printf("\tret\n");
    return 0;
}