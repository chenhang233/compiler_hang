#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int Putback;
int Line;
FILE *Infile;

enum
{
    T_ADD,
    T_SUB,
    T_MUT,
    T_DIV,
    T_INTLIT
};

char *token_map[] = {"+", "-", "*", "/", "intlit"};

struct Token
{
    int token;
    int intValue;
};

int scan(struct Token *);
static int scanInt(int);
static int skip();
static int next();
static int strIntPos(char *, char);