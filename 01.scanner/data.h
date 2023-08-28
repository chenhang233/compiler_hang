#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  + - * / int
enum
{
    T_PUT,
    T_SUB,
    T_MUL,
    T_DIV,
    T_INTLIT
};

extern int Line;
extern int Putback;
extern FILE *Infile;

struct Token
{
    int token;
    int intValue;
};

int scan(struct Token *t);
