#include "function.h"

// statements.c

AST_node *print_statement(void)
{
    AST_node *root;
    match_print();
    root = parse_ast_expr(0);
    root = mkAST_left(A_PRINT, root, 0);
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
    right = mkAST_leaf(A_LVIDENT, id);
    match_assign();
    left = parse_ast_expr(0);
    root = mkAST_node(A_ASSIGN, left, NULL, right, 0);
    return root;
}

AST_node *for_statement()
{
    AST_node *left;
    AST_node *prev, *cond, *post, *body;
    match_for();
    match_lparen();
    prev = single_statement();
    match_semi();
    cond = parse_ast_expr(0);
    match_semi();
    post = single_statement();
    match_rparen();
    body = compound_statement();

    left = mkAST_node(A_GLUE, body, NULL, post, 0);
    left = mkAST_node(A_WHILE, cond, NULL, left, 0);
    left = mkAST_node(A_GLUE, prev, NULL, left, 0);
    return left;
}

static AST_node *single_statement()
{
    switch (t_instance.token)
    {
    case T_INT:
        var_declaration();
        return NULL;
    case T_IDENT:
        return assignment_statement();
    case T_PRINT:
        return print_statement();
    case T_IF:
        return if_statement();
    case T_WHILE:
        return while_statement();
    case T_FOR:
        return for_statement();
    default:
        custom_error_int("Syntax error token", t_instance.token);
    }
}

AST_node *compound_statement(void)
{
    AST_node *left = NULL;
    AST_node *tree;
    match_lbrace();
    while (1)
    {
        tree = single_statement();
        if (tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN))
            match_semi();
        if (tree)
        {
            if (!left)
                left = tree;
            else
                left = mkAST_node(A_GLUE, left, NULL, tree, 0);
        }
        if (t_instance.token == T_RBRACE)
        {
            match_rbrace();
            return left;
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

AST_node *function_declaration()
{
    AST_node *tree;
    match_void();
    match_ident();
    int id = addglob(Text);
    match_lparen();
    match_rparen();
    tree = compound_statement();
    return mkAST_left(A_FUNCTION, tree, id);
}

AST_node *if_statement(void)
{
    AST_node *condition_tree, *if_tree, *else_tree = NULL;
    match_if();
    match_lparen();
    condition_tree = parse_ast_expr(0);
    if (condition_tree->op < T_EQ || condition_tree->op > T_GE)
    {
        custom_error_int("Bad comparison operator", condition_tree->op);
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

AST_node *while_statement()
{
    AST_node *condition_tree, *body;
    match_while();
    match_lparen();
    condition_tree = parse_ast_expr(0);
    if (condition_tree->op < T_EQ || condition_tree->op > T_GE)
    {
        custom_error_int("Bad comparison operator", condition_tree->op);
    }
    match_rparen();
    body = compound_statement();
    return mkAST_node(A_WHILE, condition_tree, NULL, body, 0);
}
