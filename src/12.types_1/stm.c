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
    ASTnode *tree;
    Primitive_type l, r;

    match_print();
    tree = binexpr(0);
    l = P_INT;
    r = tree->type;
    if (!type_compatible(&l, &r, 0))
        custom_error_int("Incompatible types", 0);
    if (r)
        tree = mkAST_left(r, P_INT, tree, 0);
    tree = mkAST_left(A_PRINT, P_NONE, tree, 0);
    return tree;
}

Primitive_type parse_type(Token_type t)
{
    if (t == T_CHAR)
        return (P_CHAR);
    if (t == T_INT)
        return (P_INT);
    if (t == T_VOID)
        return (P_VOID);
    fatald("Illegal type, token", t);
}

void var_declaration()
{
    Primitive_type pt = parse_type(t_instance.token);
    scan(&t_instance);
    int id = addglob(Text, pt, S_VARIABLE);
    cgglobsym(id);
    match_semi();
};

static ASTnode *assignment_statement(void)
{
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