#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma once

#define TEXT_LEN 1024
#define GLOBAL_SYMBOOL_LEN 1024

typedef enum Token_type
{
    T_EOF,
    T_ADD,
    T_SUB,
    T_MUL,
    T_DIV,
    T_INTLIT,
    T_SEMI,
    T_EQUALS,
    T_IDENT,
    T_PRINT,
    T_INT
} Token_type;

typedef struct Token
{
    Token_type type;
    int v;
} Token;

typedef enum AST_NODE_TYPE
{
    A_ADD,
    A_SUB,
    A_MUL,
    A_DIV,
    A_INTLIT,
    A_IDENT,
    A_IDENT2,
    A_ASSIGN
} AST_TYPE;

typedef union AST_V
{
    int int_v;
    int id;
} AST_V;

typedef struct AST_NODE
{
    AST_TYPE op;
    struct AST_NODE *l;
    struct AST_NODE *r;
    AST_V v;
} AST;

typedef struct Symbol_table_structure
{
    char *name;
} Symbol_table_structure;

int Line;
int Cache_c;
FILE *Infile;
FILE *Outfile;
char TEXT[TEXT_LEN];
Token T_instance;
Symbol_table_structure Global_symbol_map[GLOBAL_SYMBOOL_LEN];