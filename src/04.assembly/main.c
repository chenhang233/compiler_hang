#include "data.h"

void usage(char *perf, char const *info)
{
    printf("%s ERROR: %s", perf, info);
    exit(1);
}

int next()
{
    int c;
    if (InfileCache)
    {
        c = InfileCache;
        InfileCache = 0;
        return c;
    }
    c = fgetc(Infile);
    if (c == '\n')
    {
        Line++;
    }
    return c;
}

int skip()
{
    int c = next();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t')
    {
        c = next();
    }
    return c;
}

int strPosInt(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scanDigit(int c)
{
    int t, v = 0;
    while ((t = strPosInt("0123456789", c)) >= 0)
    {
        v = v * 10 + t;
        c = next();
    }
    InfileCache = c;
    return v;
}

int scan(Token *t)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        t->token = T_EOF;
        return 0;
    case '+':
        t->token = T_ADD;
        break;
    case '-':
        t->token = T_SUB;
        break;
    case '*':
        t->token = T_MUT;
        break;
    case '/':
        t->token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->intValue = scanDigit(c);
            break;
        }
        usage("Invalid character", (char *)&c);
    }
    return 1;
}

void init()
{
    InfileCache = '\n';
    Line = 1;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc length", argv[0]);
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open Infile", argv[1]);
    }
    scan(&T);
    return 0;
}

ASTnode *mkASTnode(ASTtype op, struct ASTnode *l, struct ASTnode *r, int v)
{
    ASTnode *root = malloc(sizeof(ASTnode *));
    if (root == NULL)
    {
        usage("malloc", "NULL");
    }
    root->l = l;
    root->r = r;
    root->op = op;
    root->v = v;
    return root;
}

ASTnode *mkASTleaf(ASTtype t, int v)
{
    return mkASTnode(t, NULL, NULL, v);
}