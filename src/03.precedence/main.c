#include "data.h"

void usage(char *perf, const char *msg)
{
    printf("%s ERROR: %s\n", perf, msg);
    exit(1);
}

void init()
{
    PutCache = '\n';
    Line = 1;
}

int next()
{
    int c;
    if (PutCache)
    {
        c = PutCache;
        PutCache = 0;
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
    while (c == '\n' || c == '\t' || c == '\r' || c == ' ' || c == '\f')
    {
        c = next();
    }
    return c;
}

int scan(struct Token *T)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        token.token = T_EOF;
        return 0;
    case '+':
        token.token = T_ADD;
        break;
    case '-':
        token.token = T_SUB;
        break;
    case '*':
        token.token = T_MUT;
        break;
    case '/':
        token.token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            token.token = T_INTLIT;
            token.intValue = scanInt(c);
        }
        break;
    }
}

int scanInt(char c)
{
    int t, v = 0;
    while ((t = strPosInt("0123456789", c)) >= 0)
    {
        v = v * 10 + t;
        c = next();
    }
    PutCache = c;
    return v;
}

int strPosInt(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

struct ASTnode *mkASTroot(enum AstType op, struct ASTnode *l, struct ASTnode *r, int v)
{
    struct ASTnode *root = malloc(sizeof(struct ASTnode));
    if (root == NULL)
    {
        usage("malloc", "mkASTroot()");
    }
    root->op = op;
    root->left = l;
    root->right = r;
    root->intValue = v;
    return root;
}

struct ASTnode *mkASTleaf(enum AstType op, int v)
{
    return mkASTroot(op, NULL, NULL, v);
}

struct ASTnode *primary()
{
    switch (token.token)
    {
    case T_INTLIT:
        return mkASTleaf(A_INTLIT, token.intValue);

    default:
        usage("T_INTLIT", "primary");
    }
}

int opPrecedence(enum TokenType tt)
{
    int f = OperatorPrecedence[tt];
    if (f == 0)
    {
        usage("syntax", (char *)&Line);
    }
    return f;
}

enum AstType opType(enum TokenType tt)
{
    switch (tt)
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
        usage("op", " ");
    }
}

struct ASTnode *binexpr(int pre)
{
    struct ASTnode *left, *right;
    enum TokenType tt;
    left = primary();
    scan(&token);
    tt = token.token;
    if (tt == T_EOF)
    {
        return left;
    }
    while (opPrecedence(tt) > pre)
    {
        scan(&token);
        right = binexpr(OperatorPrecedence[tt]);
        left = mkASTroot(opType(tt), left, right, 0);
        tt = token.token;
        if (tt == T_EOF)
        {
            return left;
        }
    }
    return left;
}

void prNode(struct ASTnode *node)
{
    printf("value=%d op=%d\n", node->intValue, node->op);
    if (node->left)
    {
        prNode(node->left);
    }
    if (node->right)
    {
        prNode(node->right);
    }
}

int interpretAST(struct ASTnode *node)
{
    int left, right;
    if (node->left)
        left = interpretAST(node->left);
    if (node->right)
        right = interpretAST(node->right);
    if (node->op == A_INTLIT)
        printf("int: %d\n", node->intValue);
    else
        printf("%d %s %d\n", left, ASTopMap[node->op], right);
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
        usage("Unknown AST operator", (char *)&node->op);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc length", argv[0]);
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open source code", argv[1]);
    }
    init();
    scan(&token);
    struct ASTnode *node = binexpr(0);
    printf("%d \n", interpretAST(node));
    return 0;
}
