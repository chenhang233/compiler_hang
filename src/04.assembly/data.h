#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define REG_LEN 4

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
FILE *Outfile;
Token T;
int Line;
int InfileCache;
const char tokenWeights[] = {0, 10, 10, 20, 20, 0};
const char *RegisterList[REG_LEN] = {"%r8", "%r9", "%r10", "%r11"};
int RegisterStatus[REG_LEN];

void init();
void usage(char *, char const *);
int scan(Token *);
int skip();
int next();
int scanDigit(int);
int strPosInt(char *, char);
ASTnode *mkASTnode(ASTtype, struct ASTnode *, struct ASTnode *, int);
ASTnode *mkASTleaf(ASTtype, int);
ASTnode *parseAST(int);
ASTnode *parseInt();
int calcScore(int);
ASTtype transType(TokenType);
int interpretAST(ASTnode *);
void generatecode(ASTnode *);
void RegisterFree(int);
int RegisterAlloc();
void cgpreamble();
void cgpostamble();
int generateAST(struct ASTnode *);
void callprintint(int r);
int callLoad(int);
int callAdd(int, int);
int callSub(int, int);
int callMut(int, int);
int callDiv(int, int);
