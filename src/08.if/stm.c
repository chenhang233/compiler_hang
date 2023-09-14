#include "function.h"

// statements.c

AST_node *print_statement(void)
{
    AST_node *root;
    match_print();
    root = parse_ast_expr(0);
    root = mkAST_left(A_PRINT, root);
    match_semi();
    return root;
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
        case T_PRINT:
            tree = print_statement();
            break;
        case T_IF:
            tree = if_statement();
            break;
        default:
            custom_error_int("Syntax error, token", t_instance.token);
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
    AST_node *condition_tree, *if_tree, *else_tree = NULL;
    match_if();
    match_lparen();
    condition_tree = parse_ast_expr(0);
    if (condition_tree < T_EQ || condition_tree > T_GE)
    {
        custom_error_int("Bad comparison operator", condition_tree);
    }
    match_rparen();
    if_tree = compound_statement();
    if (t_instance.token == T_ELSE)
    {
        scan(&t_instance);
        else_tree = compound_statement();
    }
    return mkAST_node(A_IF, condition_tree, if_tree, else_tree, 0);
}