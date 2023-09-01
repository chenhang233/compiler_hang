#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#pragma once

typedef enum TokenType
{
    T_EOF,
    T_ADD,
    T_SUB,
    T_MUT,
    T_DIV,
    T_INTLIT
} TokenType;

typedef enum ASTtype
{
    A_ADD,
    A_SUB,
    A_MUT,
    A_DIV,
    A_INTLIT
} ASTtype;

typedef struct Token
{
    TokenType token;
    int intValue;
} Token;

typedef struct ASTnode
{
    struct ASTnode *l;
    struct ASTnode *r;
    ASTtype op;
    int v;
} ASTnode;

FILE *Infile;
Token T;
int Line;
int InfileCache;

void init();
void usage(char *, char const *);
int scan(Token *);
int skip();
int next();
int scanDigit(int);
int strPosInt(char *, char);
ASTnode *mkASTnode(ASTtype, struct ASTnode *, struct ASTnode *, int);
ASTnode *mkASTleaf(ASTtype, int);