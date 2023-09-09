#include "function.h"

static const int token_type_score[] = {0, 10, 10, 20, 20, 0};

AST *primary()
{
    AST *node;
    int id;
    AST_V v;
    switch (T_instance.type)
    {
    case T_INTLIT:
        v.int_v = T_instance.v;
        node = mkast_leaf(A_INTLIT, v);
        break;
    case T_IDENT:
        if ((id = global_find(TEXT)) == -1)
        {
            custom_error_chars("Unknown variable", TEXT);
        };
        v.id = id;
        node = mkast_leaf(A_IDENT, v);
        break;
    default:
        custom_error_int("yntax error, token", T_instance.type);
    }
    scan(&T_instance);
    return node;
}

int get_score(Token_type t)
{
    int c = token_type_score[t];
    if (c == 0)
    {
        custom_error_int("Syntax error, token", t);
    }
    return c;
}

static AST_TYPE arithop(Token_type tokentype)
{
    switch (tokentype)
    {
    case T_ADD:
        return A_ADD;
    case T_SUB:
        return A_SUB;
    case T_MUL:
        return A_MUL;
    case T_DIV:
        return A_DIV;
    default:
        custom_error_int("Syntax error, Token_type: ", tokentype);
    }
}

AST *ast_generator(int prev)
{
    AST *l, *r;
    l = primary();
    Token_type op;
    op = T_instance.type;
    if (op == T_SEMI)
    {
        return l;
    }
    while (get_score(op) > prev)
    {
        scan(&T_instance);
        r = ast_generator(token_type_score[op]);
        AST_V v;
        l = mkast_node(arithop(op), l, r, v);
        op = T_instance.type;
        if (op == T_SEMI)
        {
            return l;
        }
    }
    return l;
}