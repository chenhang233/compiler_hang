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
enum astType
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
    enum astType op;
    struct ASTnode *left;
    struct ASTnode *right;
    int intValue;
};

int InputCache;
int Line;
FILE *Infile;
struct Token T;
char *astOpMap[] = {"+", "-", "*", "/"};

void usage(char *, const char *);
void init();
int scan(struct Token *);
int skip();
int next();
int scanInt(int);
int strIntPos(char *, char);
struct ASTnode *binexpr();
struct ASTnode *primary();
struct ASTnode *mkASTnode(enum astType, struct ASTnode *, struct ASTnode *, int);
struct ASTnode *mkASTleaf(enum astType, int);
enum astType mkASTtype(enum TokenType);
int interpretAST(struct ASTnode *);
