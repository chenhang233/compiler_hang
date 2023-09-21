#include "function.h"

void custom_error_line()
{
    printf("The source code error is on line %d\n", Line);
    exit(1);
}

void custom_error_char(char *t, char c)
{
    printf("ERROR: %s ,char: %c\n", t, c);
    custom_error_line();
}

void custom_error_chars(char *c1, char *c2)
{
    printf("ERROR: %s ,char*: %s\n", c1, c2);
    custom_error_line();
};

void custom_error_int(char *c1, int i)
{
    printf("ERROR: %s ,int: %d\n", c1, i);
    custom_error_line();
}

void match(AST_node_type t, char *str)
{
    if (t_instance.token == t)
    {
        scan(&t_instance);
    }
    else
    {
        custom_error_chars("connot match", str);
    }
}

void match_semi()
{
    return match(T_SEMI, ";");
}

void match_lbrace()
{
    match(T_LBRACE, "{");
}

void match_rbrace()
{
    match(T_RBRACE, "}");
}

void match_lparen()
{
    match(T_LPAREN, "(");
}

void match_rparen()
{
    match(T_RPAREN, ")");
}

void match_ident()
{
    match(T_IDENT, "identifier");
}

void match_int()
{
    match(T_INT, "int");
}

void match_assign()
{
    match(T_ASSIGN, "assign");
}

void match_print()
{
    match(T_PRINT, "print");
}

void match_if()
{
    match(T_IF, "if");
}

void match_while()
{
    match(T_WHILE, "while");
}

void match_for()
{
    match(T_FOR, "for");
}

void match_void()
{
    match(T_VOID, "void");
}
