#include "function.h"

ASTnode *mkAST_node(AST_node_type op, Primitive_type type,
                    ASTnode *left, ASTnode *mid, ASTnode *right, int intv)
{
    ASTnode *n = malloc(sizeof(ASTnode));
    if (!n)
        custom_error_char("unable malloc", 0);
    n->op = op;
    n->type = type;
    n->left = left;
    n->right = right;
    n->mid = mid;
    n->v.intvalue = intv;
    return n;
};
ASTnode *mkAST_leaf(AST_node_type op, Primitive_type type, int id)
{
    return mkAST_node(op, type, NULL, NULL, NULL, id);
};
ASTnode *mkAST_left(AST_node_type op, Primitive_type type, ASTnode *left,
                    int v)
{
    return mkAST_node(op, type, left, NULL, NULL, v);
    ;
};

static int OpPrec[] = {
    0, 10,         // T_EOF,  T_ASSIGN
    20, 20,        // T_PLUS, T_MINUS
    30, 30,        // T_STAR, T_SLASH
    40, 40,        // T_EQ, T_NE
    50, 50, 50, 50 // T_LT, T_GT, T_LE, T_GE
};

static int op_precedence(Token_type tokentype)
{
    int prec = OpPrec[tokentype];
    if (prec == 0)
        custom_error_int("Syntax error, token", tokentype);
    return (prec);
}

static int rightassoc(Token_type tokentype)
{
    if (tokentype == T_ASSIGN)
        return (1);
    return (0);
}
//  binary ast operator
static AST_node_type binastop(Token_type tokentype)
{
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return (tokentype);
    custom_error_int("Syntax error, token", tokentype);
}

ASTnode *binexpr(int ptp)
{
    ASTnode *left, *right;
    ASTnode *l_temp, *r_temp;
    left = prefix();
    Token_type type = t_instance.token;
    AST_node_type ast_type;
    if (type == T_SEMI || type == T_RPAREN)
    {
        left->rvalue = 1;
        return left;
    }
    while (op_precedence(type) > ptp || (rightassoc(type) && op_precedence(type) == ptp))
    {
        right = binexpr(OpPrec[type]);
        ast_type = binastop(type);
        if (type == T_ASSIGN)
        {
            right->rvalue = 1;
            right = modify_type(right, left->type, 0);
            if (!right)
                custom_error_char("Incompatible expression in assignment", 0);
            l_temp = left;
            left = right;
            right = l_temp;
        }
        else
        {
            left->rvalue = 1;
            right->rvalue = 1;
            l_temp = modify_type(left, right->type, ast_type);
            r_temp = modify_type(right, left->type, ast_type);
            if (!l_temp && !r_temp)
                custom_error_int("Incompatible types in binary expression", 0);
            if (l_temp)
                left = l_temp;
            if (r_temp)
                right = r_temp;
        }
        left = mkAST_node(ast_type, left->type, left, NULL, right, 0);
        type = t_instance.token;
        if (type == T_SEMI || type == T_RPAREN)
        {
            left->rvalue = 1;
            return left;
        }
    }
    left->rvalue = 1;
    return left;
}

static ASTnode *primary(void)
{
    ASTnode *n;
    int v;
    switch (t_instance.token)
    {
    case T_INTLIT:
        v = t_instance.intvalue;
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
    ASTnode *tree;
    Primitive_type type;
    switch (t_instance.token)
    {
    case T_AMPER:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_ADDR)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree->op = A_ADDR;
        tree->type = pointer_to(tree->type);
        break;
    case T_STAR:
        scan(&t_instance);
        tree = prefix();
        if (tree->op != A_IDENT && tree->op != A_DEREF)
            custom_error_int("Get an address that is not an A_IDENT", tree->op);
        tree = mkAST_left(A_DEREF, value_at(tree->type), tree, 0);
        break;
    default:
        tree = primary();
    }
    return tree;
}

ASTnode *funccall(void)
{
    ASTnode *tree;
    int id;
    if (findglob(Text) == -1)
        custom_error_chars("undefined variable", Text);

    match_lparen();
    tree = binexpr(0);
    tree = mkAST_left(A_FUNCCALL, Gsym[id].type, tree, id);
    match_rparen();

    return tree;
}
