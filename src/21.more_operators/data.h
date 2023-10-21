#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma once

#define TEXTLEN 1024
#define NOREG -1
#define NOLABEL 0 // Use NOLABEL when we have no label to

typedef enum Token_type
{
    T_EOF, //  eof
    // Binary operators
    T_ASSIGN, // assign =
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
    // Other operators
    T_INC,    //++
    T_DEC,    //--
    T_INVERT, //!
    T_LOGNOT, //
    // Type keywords
    T_VOID, // void
    T_CHAR, // char
    T_INT,  // int
    T_LONG, // long
    // Structural tokens
    T_INTLIT,   // digit
    T_STRLIT,   // string
    T_SEMI,     // ;
    T_IDENT,    //  identify
    T_LBRACE,   // {
    T_RBRACE,   // }
    T_LPAREN,   // (
    T_RPAREN,   // )
    T_LBRACKET, // [
    T_RBRACKET, // ]
    T_AMPER,    // &
    T_LOGAND,   // &&
    // Other keywords
    T_IF,     // if
    T_ELSE,   // else
    T_WHILE,  // while
    T_FOR,    // for
    T_RETURN, // return

} Token_type;

typedef enum Structural_type
{
    S_VARIABLE,
    S_FUNCTION,
    S_ARRAY
} Structural_type;

typedef enum Primitive_type
{
    P_NONE,
    P_VOID,
    P_CHAR,
    P_INT,
    P_LONG,
    P_VOIDPTR,
    P_CHARPTR,
    P_INTPTR,
    P_LONGPTR
} Primitive_type;

typedef enum AST_node_type
{
    A_ASSIGN = 1,
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ,
    A_NE,
    A_LT,
    A_GT,
    A_LE,
    A_GE,
    A_INTLIT, // 12
    A_STRLIT,
    A_IDENT, // 14
    A_GLUE,
    A_IF,
    A_WHILE,
    A_FUNCTION,
    A_WIDEN,
    A_RETURN,
    A_FUNCCALL,
    A_DEREF,
    A_ADDR,
    A_SCALE
} AST_node_type;

typedef struct ASTnode
{
    AST_node_type op;
    Primitive_type type;  // Type of any expression this tree generates
    int rvalue;           // True if the node is an rvalue
    struct ASTnode *left; // Left, middle and right child trees
    struct ASTnode *mid;
    struct ASTnode *right;
    union
    {
        int intvalue; // For A_INTLIT, the integer value
        int id;       // For A_IDENT, the symbol slot number
        int size;     // For A_SCALE, the size to scale by
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
    int endlabel;          // For S_FUNCTIONs, the end label
    int size;              // Number of elements in the symbol
} symtable;

int Line;
int cache;
char Text[TEXTLEN];
int Functionid; // Symbol id of the current function
int DumpAST;    // debug ast

Token t_instance;
FILE *Infile;
FILE *Outfile;
symtable Gsym[TEXTLEN]; // global symbol table