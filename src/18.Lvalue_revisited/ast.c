#include "function.h"

ASTnode *binexpr(int ptp)
{
    ASTnode *left, *right;
    ASTnode *l_temp, *r_temp;
}

static ASTnode *primary(void)
{
    ASTnode *n;

    switch (t_instance.token)
    {
    case T_INTLIT:
        int v = t_instance.intvalue;
        if (v >= 0 && v < 256)
            n = mkAST_leaf(A_INTLIT, P_CHAR, v);
        else
            n = mkAST_leaf(A_INTLIT, P_INT, v);
        break;
    case T_IDENT:
        scan(&t_instance);
        if (t_instance.token == T_LPAREN)
            return funccall();
        reject_token(&t_instance);
        int id = findglob(Text);
        if (id == -1)
            custom_error_int("unknown variable", id);
        n = mkAST_leaf(A_IDENT, Gsym[id].type, id);
        break;
    default:
        custom_error_int("syntax error, token: ", t_instance.token);
    }
    scan(&t_instance);
    return n;
}

ASTnode *prefix(void)
{
}

ASTnode *funccall(void)
{
}
