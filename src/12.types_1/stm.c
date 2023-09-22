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
    custom_error_int("Illegal type, token", t);
}

void var_declaration()
{
    Primitive_type pt = parse_type(t_instance.token);
    scan(&t_instance);
    match_ident();
    int id = addglob(Text, pt, S_VARIABLE);
    cgglobsym(id);
    match_semi();
    printf("token=%d\n", t_instance.token);
};

static ASTnode *assignment_statement(void)
{
    ASTnode *left, *right, *root;
    int id;
    Primitive_type lt, rt;
    match_ident();
    if ((id = findglob(Text)) == -1)
        custom_error_chars("undefined variable", Text);
    right = mkAST_leaf(A_LVIDENT, Gsym[id].type, id);
    match_assign();
    left = binexpr(0);
    lt = left->type;
    rt = right->type;
    if (!type_compatible(&lt, &rt, 1))
        custom_error_int("Incompatible types", 0);
    if (lt)
        left = mkAST_left(left->type, right->type, left, 0);
    root = mkAST_node(A_ASSIGN, P_INT, left, NULL, right, 0);
    return root;
}

static ASTnode *if_statement()
{
    ASTnode *cond, *true_tp, *false_tp;
    match_if();
    match_lparen();
    cond = binexpr(0);
    if (cond->op < A_EQ || cond->op > A_GE)
        custom_error_int("Bad comparison operator", cond->op);
    match_rparen();

    true_tp = compound_statement();

    if (t_instance.token == T_ELSE)
    {
        scan(&t_instance);
        false_tp = compound_statement();
    }
    return mkAST_node(A_IF, P_NONE, cond, true_tp, false_tp, 0);
}

static ASTnode *while_statement()
{
    ASTnode *cond, *body;
    match_while();
    match_lparen();
    cond = binexpr(0);
    if (cond->op < A_EQ || cond->op > A_GE)
        custom_error_int("condition expression", cond->op);
    match_rparen();
    body = compound_statement();
    return mkAST_node(A_WHILE, P_NONE, cond, NULL, body, 0);
}
static ASTnode *for_statement()
{
    ASTnode *pre, *cond, *body, *post;
    ASTnode *tree;
    match_for();
    match_lparen();
    pre = single_statement();
    match_semi();
    cond = binexpr(0);

    if (cond->op < A_EQ || cond->op > A_GE)
        custom_error_int("Bad comparison operator", cond->op);
    match_semi();

    post = single_statement();
    match_rparen();
    body = compound_statement();
    tree = mkAST_node(A_GLUE, P_NONE, body, NULL, post, 0);
    tree = mkAST_node(A_WHILE, P_NONE, cond, NULL, tree, 0);
    tree = mkAST_node(A_GLUE, P_NONE, pre, NULL, tree, 0);

    return tree;
}

static ASTnode *single_statement(void)
{
    printf("t_instance.token=%d\n", t_instance.token);
    switch (t_instance.token)
    {
    case T_PRINT:
        return print_statement();
    case T_INT:
    case T_CHAR:
        var_declaration();
        return NULL;
    case T_IDENT:
        return assignment_statement();
    case T_IF:
        return if_statement();
    case T_WHILE:
        return while_statement();
    case T_FOR:
        return for_statement();
    default:
        custom_error_char("147 Syntax error, token", t_instance.token);
    }
}

ASTnode *compound_statement()
{
    match_lbrace();
    ASTnode *tree, *left = NULL;
    while (1)
    {
        tree = single_statement();
        if (tree && (tree->op == A_PRINT || tree->op == A_ASSIGN))
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