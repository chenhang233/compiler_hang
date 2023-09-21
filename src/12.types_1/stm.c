#include "function.h"

ASTnode *function_declaration()
{
    ASTnode *tree;
    int name_id;
    match_void();
    match_ident();
    name_id = addglob(Text, P_VOID, S_FUNCTION);
    match_lparen();
    match_rparen();
    tree = compound_statement();
    return mkAST_left(A_FUNCTION, P_VOID, tree, name_id);
}

static ASTnode *print_statement(void)
{
    match_print();
}

static ASTnode *single_statement(void)
{
    switch (t_instance.token)
    {
    case T_PRINT:
        return print_statement();
    case T_INT:
    case T_CHAR:
        var_declaration();
        break;
    case T_ASSIGN:
        return assignment_statement();
    case T_IF:
        return if_statement();
    case T_WHILE:
        return while_statement();
    case T_FOR:
        return for_statement();
    default:
        custom_error_char("Syntax error, token", t_instance.token);
    }
}

ASTnode *compound_statement()
{
    match_lbrace();
    ASTnode *tree, *left = NULL;
    while (1)
    {
        tree = single_statement();
        if (tree->op == A_PRINT || tree->op == A_ASSIGN)
            match_semi();
        if (tree)
        {
            if (!left)
                left = tree;
            else
                left = mkAST_node(A_GLUE, P_NONE, left, NULL, tree, 0);
        }
        if (t_instance.token == T_RBRACE)
        {
            match_rbrace();
            return left;
        }
    }
}