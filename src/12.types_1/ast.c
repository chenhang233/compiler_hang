#include "function.h"

ASTnode *mkAST_node(AST_node_type op, Primitive_type type, ASTnode *left, ASTnode *mid, ASTnode *right, int intv)
{
    ASTnode *node = malloc(sizeof(ASTnode));
    if (node == NULL)
        custom_error_int("malloc", 0);
    node->op = op;
    node->type = type;
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->v.intvalue = intv;
    return node;
}

ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, int id)
{
    return mkAST_node(op, type, NULL, NULL, NULL, id);
}

ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left, int v)
{
    return mkAST_node(op, type, left, NULL, NULL, v);
}

static int OpPrec[] = {
    0, 10, 10,     // T_EOF, T_PLUS, T_MINUS
    20, 20,        // T_STAR, T_SLASH
    30, 30,        // T_EQ, T_NE
    40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

static int arithop(int tokentype)
{
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return (tokentype);
    custom_error_int("Syntax error", tokentype);
}

static int op_precedence(AST_node_type tokentype)
{
    int prec = OpPrec[tokentype];
    if (prec == 0)
        fatald("Syntax error, token", tokentype);
    return (prec);
}

static ASTnode *primary(void)
{
    ASTnode *n;
    switch (t_instance.token)
    {
    case T_INTLIT:
        if (t_instance.intvalue >= 0 && t_instance.intvalue < 256)
            n = mkAST_leaf(A_INTLIT, P_CHAR, t_instance.intvalue);
        else
            n = mkAST_leaf(A_INTLIT, P_INT, t_instance.intvalue);
        break;
    case T_IDENT:
        int id = findglob(Text);
        if (id == -1)
            custom_error_chars("undefined variable", Text);
        n = mkAST_leaf(A_IDENT, Gsym[id].type, id);
        break;
    default:
        custom_error_int("primary token", t_instance.token);
    }
    scan(&t_instance);
    return n;
};

ASTnode *binexpr(int ptp)
{
    ASTnode *left, *right;
    AST_node_type tp;
    Primitive_type lefttype, righttype;

    left = primary();
    tp = t_instance.token;
    if (tp == T_EOF || tp == T_RPAREN)
        return left;

    while (op_precedence(tp) > ptp)
    {
        scan(&t_instance);
        right = binexpr(OpPrec[tp]);
        lefttype = left->type;
        righttype = right->type;
        if (!type_compatible(&lefttype, &righttype, 0))
            custom_error_int("Incompatible types", 0);
        if (lefttype)
            left = mkAST_left(lefttype, right->type, left, 0);
        if (righttype)
            right = mkAST_left(righttype, left->type, right, 0);
        left = mkAST_node(arithop(tp), left->type, left, NULL, right, 0);
        tp = t_instance.token;
        if (tp == T_EOF || tp == T_RBRACE)
            break;
    }
    return left;
}