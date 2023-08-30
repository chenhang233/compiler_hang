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

int PutCache;
int Line;
FILE *Infile;
struct Token token;
int OperatorPrecedence[] = {0, 10, 10, 20, 20, 0};
char *ASTopMap[] = {"+", "-", "*", "/"};

void usage(char *, const char *);
void init();
int scan(struct Token *);
int next();
int skip();
int strPosInt(char *, char);
int scanInt(char);
int opPrecedence(enum TokenType);
enum AstType opType(enum TokenType);
struct ASTnode *binexpr(int);
struct ASTnode *primary();
struct ASTnode *mkASTroot(enum AstType, struct ASTnode *, struct ASTnode *, int);
struct ASTnode *mkASTleaf(enum AstType, int);