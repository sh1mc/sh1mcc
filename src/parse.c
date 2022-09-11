#include <stdio.h>
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

Token *consume_ident() {
    if (token->kind != TK_IDENT) {
        return false;
    }
    Token *ret_tok = token;
    token = token->next;
    return ret_tok;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        error_at(token->str, "Unexpected token: %s", op);
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
bool at_num() { return token->kind == TK_NUM; }

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_var() {
    Node *node = malloc(sizeof(Node));
    node->kind = ND_LVAR;
    return node;
}

void program();
Node *statement();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

Node *code[MAX_PROGRAM_LEN];

void program() {
    int i = 0;
    while (!at_eof()) {
        code[i++] = statement();
    }
    code[i] = NULL;
}

Node *statement() {
    Node *node = expr();
    expect(";");
    return node;
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = equality();
    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *equality() {
    Node *node = relational();
    while (1) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NEQ, node, relational());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();
    while (1) {
        if (consume("<")) {
            node = new_node(ND_LESS, node, add());
        } else if (consume("<=")) {
            node = new_node(ND_LESS_EQ, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LESS, add(), node);
        } else if (consume(">=")) {
            node = new_node(ND_LESS_EQ, add(), node);
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();
    while (1) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *mul() {
    Node *node = unary();
    while (1) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("+")) {
        return primary();
    }
    if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    if (at_num()) {
        return new_node_num(expect_number());
    }
    Token *ident_tok = consume_ident();
    if (ident_tok) {
        Node *node = malloc(sizeof(Node));
        node->kind = ND_LVAR;
        node->offset = (ident_tok->str[0] - 'a' + 1) * 8;
        return node;
    }
    error("Parse Failed.");
    return NULL;
}
