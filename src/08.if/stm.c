#include "function.h"

// statements.c

AST_node *print_statement(void)
{
}

AST_node *assignment_statement(void)
{
    AST_node *left, *right, *root;
    int id;
    match_ident();
    if ((id = findglob(Text)) == -1)
    {
        custom_error_chars("Undeclared variable", Text);
    }
    left = mkAST_leaf(A_LVIDENT, id);
    match_assign();
    right = parse_ast_expr(0);
    root = mkAST_node(A_ASSIGN, left, NULL, right, 0);
    match_semi();
    return root;
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
        case T_IDENT:
            tree = assignment_statement();
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
    addglob(Text);
    genglobsym(Text);
    match_semi();
};

AST_node *if_statement(void)
{
}