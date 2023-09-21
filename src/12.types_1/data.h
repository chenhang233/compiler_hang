#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma once

#define TEXTLEN 1024
#define NOREG -1

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
    T_ASSIGN, // assign =
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

typedef enum Primitive_type
{
    P_NONE,
    P_VOID,
    P_CHAR,
    P_INT
} Primitive_type;

typedef enum Structural_type
{
    S_VARIABLE,
    S_FUNCTION
} Structural_type;

typedef enum AST_node_type
{
    A_ADD = 1,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ,
    A_NE,
    A_LT,
    A_GT,
    A_LE,
    A_GE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN,
    A_PRINT,
    A_GLUE,
    A_IF,
    A_WHILE,
    A_FUNCTION,
    A_WIDEN
} AST_node_type;

typedef struct ASTnode
{
    AST_node_type op;
    Primitive_type type;  // Type of any expression this tree generates
    struct ASTnode *left; // Left, middle and right child trees
    struct ASTnode *mid;
    struct ASTnode *right;
    union
    {
        int intvalue; // For A_INTLIT, the integer value
        int id;       // For A_IDENT, the symbol slot number
    } v;              // For A_FUNCTION, the symbol slot number
} ASTnode;

typedef struct Token
{
    Token_type token;
    int intvalue;
} Token;

typedef struct symtable
{
    char *name;            // Name of a symbol
    Primitive_type type;   // Primitive type for the symbol
    Structural_type stype; // Structural type for the symbol
} symtable;

int Line;
int cache;
char Text[TEXTLEN];
Token t_instance;
FILE *Infile;
FILE *Outfile;
symtable Gsym[TEXTLEN]; // global symbol table