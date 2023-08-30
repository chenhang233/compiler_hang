#include "data.h"

void usage(char *per, char const *info)
{
    printf("%s ERROR: %s \n", per, info);
    exit(1);
}

void init()
{
    InputCache = '\n';
    Line = 1;
}

int scan(struct Token *T)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        T->token = T_EOF;
        return 0;
    case '+':
        T->token = T_ADD;
        break;
    case '-':
        T->token = T_SUB;
        break;
    case '*':
        T->token = T_MUT;
        break;
    case '/':
        T->token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            T->token = T_INTLIT;
            T->intValue = scanInt(c);
            break;
        }
        usage("unknown character", (char *)&c);
    }
    return 1;
}

int strIntPos(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scanInt(int c)
{
    int k, v = 0;
    while ((k = strIntPos("0123456789", c)) >= 0)
    {
        v = v * 10 + k;
        c = next();
    }
    InputCache = c;
    return v;
}

int next()
{
    int c;
    if (InputCache)
    {
        c = InputCache;
        InputCache = 0;
        return c;
    }
    c = fgetc(Infile);
    if (c == '\n')
        Line++;
    return c;
}

int skip()
{
    int c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c)
    {
        c = next();
    }
    return c;
}

enum astType mkASTtype(enum TokenType ty)
{
    switch (ty)
    {
    case T_ADD:
        return A_ADD;
    case T_SUB:
        return A_SUB;
    case T_MUT:
        return A_MUT;
    case T_DIV:
        return A_DIV;
    default:
        usage("operator", (char *)&ty);
    }
}

struct ASTnode *binexpr()
{
    struct ASTnode *node, *left, *right;
    left = primary();
    scan(&T);
    if (T.token == T_EOF)
        return left;
    enum astType op;
    op = mkASTtype(T.token);
    scan(&T);
    right = binexpr();
    node = mkASTnode(op, left, right, 0);
    return node;
}

struct ASTnode *primary()
{
    switch (T.token)
    {
    case T_INTLIT:
        return mkASTleaf(A_INTLIT, T.intValue);
    default:
        usage("input intValue", (char *)&T.intValue);
    }
}

struct ASTnode *mkASTnode(enum astType op, struct ASTnode *l, struct ASTnode *r, int v)
{
    struct ASTnode *n = malloc(sizeof(struct ASTnode));
    if (n == NULL)
        usage("malloc", "mkASTnode()");
    n->op = op;
    n->left = l;
    n->right = r;
    n->intValue = v;
    return n;
}

struct ASTnode *mkASTleaf(enum astType op, int v)
{
    return mkASTnode(op, NULL, NULL, v);
}

int interpretAST(struct ASTnode *node)
{
    int left, right;
    if (node->left)
        left = interpretAST(node->left);
    if (node->right)
        right = interpretAST(node->right);
    if (node->op == A_INTLIT)
    {
        printf("int : %d \n", node->intValue);
    }
    else
    {
        printf("%d %s %d \n", left, astOpMap[node->op], right);
    }
    switch (node->op)
    {
    case A_ADD:
        return left + right;
    case A_SUB:
        return left - right;
    case A_MUT:
        return left * right;
    case A_DIV:
        return left / right;
    case A_INTLIT:
        return node->intValue;
    default:
        usage("operater", (char *)&node->op);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        usage("argc length", argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
        usage("Infile open", argv[1]);
    init();
    scan(&T);
    // printf("T: %d %d\n", T.token, T.intValue);
    struct ASTnode *n = binexpr();
    printf("%d\n", interpretAST(n));
    return 0;
}
