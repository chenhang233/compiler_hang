#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

//  + - * / int
enum
{
    T_PUT,
    T_SUB,
    T_MUL,
    T_DIV,
    T_INTLIT
};

int Line;
int Putback;
FILE *Infile;

struct Token
{
    int token;
    int intValue;
};

int scan(struct Token *t);