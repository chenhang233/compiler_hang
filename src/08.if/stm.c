#include "function.h"

// statements.c

AST_node *print_statement(void)
{
}

AST_node *assignment_statement(void)
{
}

AST_node *compound_statement(void)
{
    AST_node *left = NULL;
    AST_node *tree;
    match_lbrace();
    while (1)
    {
        switch (t_instance.token)
        {
        case T_RBRACE:
            match_rbrace();
            return left;
        case T_INT:
            var_declaration();
            tree = NULL;
            break;
        default:
            custom_error_int("Syntax error, token", t_instance.token);
        }
    }
    if (tree)
    {
        if (!left)
        {
            left = tree;
        }
        else
        {
            left = mkAST_node(A_GLUE, left, NULL, tree, 0);
        }
    }
}

void var_declaration(void)
{
    match_int();
    match_ident();
};

AST_node *if_statement(void)
{
}