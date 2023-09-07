#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma once

#define TEXT_LEN 1024

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

typedef union Token_value
{
    int int_v;
    int id;
} Token_value;

typedef struct Token
{
    Token_type type;
    Token_value v;
} Token;

int Line;
int Cache_c;
FILE *Infile;
FILE *Outfile;
char TEXT[TEXT_LEN];
Token T_instance;
