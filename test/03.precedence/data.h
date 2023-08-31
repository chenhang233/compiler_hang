#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#pragma once

enum TokenType
{
    T_EOF,
    T_ADD,
    T_SUB,
    T_MUT,
    T_DIV,
    T_INTLIT
};

enum AstType
{
    A_ADD,
    A_SUB,
    A_MUT,
    A_DIV,
    A_INTLIT
};

struct Token
{
    enum TokenType token;
    int intValue;
};

struct ASTnode
{
    enum AstType op;
    struct ASTnode *left;
    struct ASTnode *right;
    int intValue;
};

void usage(char *, char const *);
void init();
int scan(struct Token *);
int next();
int skip();
int scanInt(int);
int strIntPos(char *, char);

struct ASTnode *mkastNode(enum AstType, struct ASTnode *, struct ASTnode *, int);
struct ASTnode *mkastLeaf(enum AstType, int);
struct ASTnode *binexpr(int);
struct ASTnode *primary();
enum AstType getASTop(enum TokenType);
int calcPreced(enum TokenType);
int interpretAST(struct ASTnode *);

int InputCache;
int Line;
FILE *Infile;
struct Token Token;

char *ASTopMap[] = {"+", "-", "*", "/"};
int tokenMap[] = {0, 10, 10, 20, 20, 0};