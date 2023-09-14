#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma once

#define NOREG -1
#define TEXT_LEN 1024

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
    T_ELSE   // else
} Token_type;

typedef struct Token
{
    Token_type token;
    int v;
} Token;

typedef enum AST_type
{
    A_ADD = 1,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ,      // ==
    A_NE,      // !=
    A_LT,      // <
    A_GT,      // >
    A_LE,      // <=
    A_GE,      //  >=
    A_INTLIT,  // 数值
    A_IDENT,   // 表达式 标识符叶子节点 id
    A_LVIDENT, // 赋值时,被赋值的 标识符id
    A_ASSIGN,  // 赋值 左->右
    A_PRINT,   // 打印
    A_GLUE,    // 花括号里面如果多条语句,黏合在一个树中
    A_IF       // if
} AST_type;

typedef struct AST_node
{
    AST_type op;
    struct AST_node *left;
    struct AST_node *mid;
    struct AST_node *right;
    union
    {
        int intv;
        int id;
    } v;

} AST_node;

typedef struct symbol_table
{
    char *name;
} symbol_table;

int Line;
int Cache;
FILE *Infile;
FILE *Outfile;
Token t_instance;
char Text[TEXT_LEN];
symbol_table Gsym[TEXT_LEN];