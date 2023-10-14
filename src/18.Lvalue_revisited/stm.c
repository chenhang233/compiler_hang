#include "function.h"

Primitive_type parse_type()
{
    Primitive_type type;
    switch (t_instance.token)
    {
    case T_VOID:
        type = P_VOID;
        break;
    case T_CHAR:
        type = P_CHAR;
        break;
    case T_INT:
        type = P_INT;
        break;
    case T_LONG:
        type = P_LONG;
        break;
    default:
        custom_error_int("Illegal type, token", t_instance.token);
    }
    while (1)
    {
        scan(&t_instance);
        printf("26--token=%d\n", t_instance.token);
        if (t_instance.token != T_STAR)
            break;
        type = pointer_to(type);
    }
    return type;
}

void global_declarations()
{
    ASTnode *tree;
    Primitive_type type;
    while (1)
    {
        type = parse_type();
        match_ident();
        if (t_instance.token == T_LPAREN)
        {
            tree = function_declaration(type);
            if (DumpAST)
            {
                // dumpAST
                // dumpAST(tree, NOLABEL, 0);
                // fprintf(stdout, "\n\n");
            }
            // genAST(tree, NOLABEL, 0);
        }
        else
        {
            var_declaration(type);
        }
        if (t_instance.token == T_EOF)
            break;
    }
}

ASTnode *function_declaration(Primitive_type type)
{
    ASTnode *tree, *final_stm;
    int name_id, label_id;
    label_id = genlabel();
    name_id = addglob(Text, type, S_FUNCTION, label_id);
    Functionid = label_id;

    match_lparen();
    match_rparen();

    tree = compound_statement();
    if (type != P_VOID)
    {
        if (!tree)
            custom_error_int("No statements in function with non-void type", 0);
        final_stm = tree->op == A_GLUE ? tree->right : tree;
        if (!final_stm || final_stm->op != A_RETURN)
            custom_error_int("No return for function with non-void type", 0);
    }
    return mkAST_left(A_FUNCTION, type, tree, name_id);
}

void var_declaration(Primitive_type type)
{
    int id = addglob(Text, type, S_VARIABLE, 0);
    genglobsym(id);
    match_semi();
}

ASTnode *compound_statement()
{
    ASTnode *tree, *left = NULL;

    match_lbrace();
    // printf("100---%d\n", t_instance.token);
    while (1)
    {
        tree = single_statement();
        if (tree && (tree->op == A_RETURN || tree->op == A_FUNCCALL || tree->op == A_ASSIGN))
        {
            match_semi();
        }

        if (tree)
        {
            if (!left)
            {
                left = tree;
            }
            else
            {
                left = mkAST_node(A_GLUE, P_NONE, left, NULL, tree, 0);
            }
        }

        if (t_instance.token == T_RBRACE)
        {
            match_rbrace();
            return left;
        }
    }
}

static ASTnode *single_statement(void)
{
    Primitive_type type;
    switch (t_instance.token)
    {
    case T_CHAR:
    case T_INT:
    case T_LONG:
        type = parse_type();
        match_ident();
        var_declaration(type);
        return NULL;
    case T_IF:
        return if_statement();
    case T_WHILE:
        return while_statement();
    case T_FOR:
        return for_statement();
    case T_RETURN:
        return return_statement();
    default:
        return binexpr(0);
    }
    return NULL;
}

static ASTnode *if_statement(void)
{
    ASTnode *cond, *trueAST, *falseAST = NULL;
    match_if();
    match_lparen();
    cond = binexpr(0);
    if (cond->op < T_EQ || cond->op > T_GE)
        custom_error_int("Bad comparison operator", cond->op);
    match_rparen();

    trueAST = compound_statement();
    if (t_instance.token == T_ELSE)
    {
        scan(&t_instance);
        falseAST = compound_statement();
    }
    return mkAST_node(A_IF, P_NONE, cond, trueAST, falseAST, 0);
};
static ASTnode *while_statement()
{
    ASTnode *cond, *body;
    match_while();
    match_lparen();
    cond = binexpr(0);
    if (cond->op < T_EQ || cond->op > T_GE)
        custom_error_int("Bad comparison operator", cond->op);
    match_rparen();

    body = compound_statement();
    return mkAST_node(A_WHILE, P_NONE, cond, NULL, body, 0);
};
static ASTnode *for_statement()
{
    ASTnode *prev, *cond, *post, *body;
    ASTnode *left;
    match_for();
    match_lparen();
    prev = single_statement();
    match_semi();

    cond = binexpr(0);
    if (cond->op < T_EQ || cond->op > T_GE)
        custom_error_int("Bad comparison operator", cond->op);
    match_semi();

    post = single_statement();
    match_rparen();

    body = compound_statement();
    left = mkAST_node(A_GLUE, P_NONE, body, NULL, post, 0);
    left = mkAST_node(A_WHILE, P_NONE, cond, NULL, left, 0);
    left = mkAST_node(A_GLUE, P_NONE, prev, NULL, left, 0);
    return left;
};
static ASTnode *return_statement(void)
{
    ASTnode *tree;
    match_return();
    Primitive_type need_type = Gsym[Functionid].type;
    if (need_type == P_VOID)
        custom_error_int("Can't return from a void function", need_type);
    match_lparen();

    tree = modify_type(tree, need_type, 0);
    if (!tree)
        custom_error_int("Incompatible type to return", 0);
    tree = binexpr(0);
    tree = mkAST_left(A_RETURN, tree->type, tree, 0);
    match_rparen();
    return tree;
};