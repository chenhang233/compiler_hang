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
        // printf("26--token=%d\n", t_instance.token);
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
            jsonify_tree(tree, "dump.json");
            if (DumpAST)
            {
                // dumpAST
                dumpAST(tree, NOLABEL, 0);
                fprintf(stdout, "\n\n");
            }
            // genAST(tree, NOLABEL, 0);
            freeloclsyms();
        }
        else
        {
            var_declaration(type, 0, 0);
            match_semi();
        }
        if (t_instance.token == T_EOF)
            break;
    }
}

static int param_declaration(void)
{
    int count = 0;
    Primitive_type t_type;
    while (t_instance.token != T_RPAREN)
    {
        t_type = parse_type();
        match_ident();
        var_declaration(t_type, 1, 1);
        count++;
        switch (t_instance.token)
        {
        case T_COMMA:
            match_comma();
            break;
        case T_RPAREN:
            break;
        default:
            custom_error_int("nexpected token in parameter list", t_instance.token);
        }
    }

    return count;
}

ASTnode *function_declaration(Primitive_type type)
{
    ASTnode *tree, *final_stm;
    int name_id, label_id, p_len;
    label_id = genlabel();
    name_id = addglob(Text, type, S_FUNCTION, label_id, 0);
    Functionid = name_id;

    match_lparen();
    p_len = param_declaration();
    printf("p_len=%d name_id=%d\n", p_len, name_id);
    Gsym[name_id].nelems = p_len;
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

void var_declaration(Primitive_type type, int islocal, int isparam)
{
    int id;
    if (t_instance.token == T_LBRACKET)
    {
        match_lbracket();
        if (t_instance.token == T_INTLIT)
        {

            if (islocal)
            {
                // addlocl(Text, pointer_to(type), S_ARRAY, 0, t_instance.intvalue);
                custom_error_int("For now, declaration of local arrays is not implemented", 0);
            }
            else
            {
                addglob(Text, pointer_to(type), S_ARRAY, 0, t_instance.intvalue);
            }
        }
        scan(&t_instance);
        match_rbracket();
    }
    else
    {
        if (islocal)
        {
            if (addlocl(Text, type, S_VARIABLE, isparam, 1) == -1)
                custom_error_chars("Duplicate local variable declaration", Text);
        }
        else
        {
            addglob(Text, type, S_VARIABLE, 0, 1);
        }
    }
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
        var_declaration(type, 1, 0);
        match_semi();
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
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, 0);
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
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, 0);
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
    // if (cond->op < T_EQ || cond->op > T_GE)
    //     custom_error_int("Bad comparison operator", cond->op);
    if (cond->op < A_EQ || cond->op > A_GE)
        cond = mkAST_left(A_TOBOOL, cond->type, cond, 0);
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
    Primitive_type need_type = Gsym[Functionid].type;
    if (need_type == P_VOID)
        custom_error_int("can not return from a void function", 1);
    match_return();
    match_lparen();
    tree = binexpr(0);

    tree = modify_type(tree, need_type, 0);
    if (!tree)
        custom_error_int("Incompatible type to return", 0);
    tree = mkAST_left(A_RETURN, tree->type, tree, 0);
    match_rparen();
    return tree;
};