#include "function.h"

ASTnode *function_declaration()
{
    ASTnode *tree, *finalstmt;
    int name_id, type, endlabel;
    type = parse_type(t_instance.token);
    match_ident();
    endlabel = label();
    name_id = addglob(Text, type, S_FUNCTION, endlabel);
    Functionid = name_id;

    match_lparen();
    match_rparen();
    tree = compound_statement();
    if (type != P_VOID)
    {
        if (tree == NULL)
            custom_error_int("No statements in function with non-void type", 1);
        finalstmt = (tree->op == A_GLUE) ? tree->right : tree;
        if (!finalstmt || finalstmt->op != A_RETURN)
            custom_error_int("No return for function with non-void type", 0);
    }
    return mkAST_left(A_FUNCTION, type, tree, name_id);
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
        custom_error_int("print_statement Incompatible types", 0);
    // printf("r type %d\n", r);
    if (r)
    {
        tree = mkAST_left(r, P_INT, tree, 0);
    }
    tree = mkAST_left(A_PRINT, P_NONE, tree, 0);
    // printf("token: %d\n", t_instance.token);
    return tree;
}

Primitive_type parse_type(Token_type t)
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
        if (t_instance.token != T_STAR)
            break;
        type = pointer_to(type);
    }
    return type;
}

void var_declaration()
{
    Primitive_type pt = parse_type(t_instance.token);
    match_ident();
    int id = addglob(Text, pt, S_VARIABLE, 0);
    genglobsym(id);
    match_semi();
};

static ASTnode *assignment_statement(void)
{
    ASTnode *left, *right, *root;
    int id;
    Primitive_type lt, rt;
    match_ident();
    // printf("t_instance.token=%d\n", t_instance.token);
    if (t_instance.token == T_LPAREN)
        return funccall();
    if ((id = findglob(Text)) == -1)
        custom_error_chars("undefined variable", Text);
    right = mkAST_leaf(A_LVIDENT, Gsym[id].type, id);
    match_assign();
    left = binexpr(0);
    lt = left->type;
    rt = right->type;
    if (!type_compatible(&lt, &rt, 1))
        custom_error_int("assignment_statement Incompatible types", 0);
    if (lt)
        left = mkAST_left(lt, right->type, left, 0);
    // printf("id62=%d %s %d %d\n", id, Text, left->v.intvalue, left->type);
    root = mkAST_node(A_ASSIGN, P_INT, left, NULL, right, 0);
    // printf("root type %d\n", root->type);
    return root;
}

static ASTnode *if_statement()
{
    ASTnode *cond, *true_tp, *false_tp = NULL;
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

static ASTnode *return_statement(void)
{
    ASTnode *tree;
    Primitive_type returntype, functype;
    if (Gsym[Functionid].type == P_VOID)
        custom_error_int("Can't return from a void function", Gsym[Functionid].type);
    match_return();
    match_lparen();
    tree = binexpr(0);
    returntype = tree->type;
    functype = Gsym[Functionid].type;
    if (!type_compatible(&returntype, &functype, 1))
        custom_error_int("return_statement Incompatible types", 1);
    if (returntype)
        tree = mkAST_left(returntype, functype, tree, 0);
    tree = mkAST_left(A_RETURN, P_NONE, tree, 0);
    match_rparen();
    return tree;
}

static ASTnode *single_statement(void)
{
    // printf("140 t_instance.token: %d\n", t_instance.token);

    switch (t_instance.token)
    {
    case T_PRINT:
        return print_statement();
    case T_INT:
    case T_CHAR:
    case T_LONG:
        // printf("149 pt---\n");
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
    case T_RETURN:
        return return_statement();
    default:
        // printf("t_instance.token: %d\n", t_instance.token);
        custom_error_int("147 Syntax error, token", t_instance.token);
    }
}

ASTnode *compound_statement()
{
    // printf("run\n");
    ASTnode *tree, *left = NULL;
    match_lbrace();
    while (1)
    {
        tree = single_statement();
        // printf("tree->op: %d\n", tree->op);
        if (tree && (tree->op == A_PRINT || tree->op == A_ASSIGN || tree->op == A_RETURN || tree->op == A_FUNCCALL))
        {
            match_semi();
        }
        if (tree)
        {
            if (!left)
                left = tree;
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