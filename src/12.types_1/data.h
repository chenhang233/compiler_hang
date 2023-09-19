#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma once

#define TEXTLEN 1024

typedef enum Token_type
{
    T_EOF,    //  eof
    T_PLUS,   //  +
    T_MINUS,  // -
    T_STAR,   //  *
    T_SLASH,  //  /
    T_EQ,     // ==
    T_NE,     // !=
    T_LT,     // <
    T_GT,     // >
    T_LE,     // <=
    T_GE,     //  >=
    T_INTLIT, // digit
    T_SEMI,   // ;
    T_ASSIGN, // assign
    T_IDENT,  //  identify
    T_LBRACE, // {
    T_RBRACE, // }
    T_LPAREN, // (
    T_RPAREN, // )
    // Keywords
    T_PRINT, // print
    T_INT,   // int
    T_IF,    // if
    T_ELSE,  // else
    T_WHILE, // while
    T_FOR,   // for
    T_VOID,  // void
    T_CHAR   // char
} Token_type;

typedef struct Token
{
    Token_type token;
    int intvalue;
} Token;

int Line;
int cache;
FILE *Infile;
FILE *Outfile;