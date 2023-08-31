#include "data.h"

void usage(char *prefix, char const *info)
{
    printf("%s ERROR: %s\n", prefix, info);
    exit(1);
}

void init()
{
    InputCache = '\n';
    Line = 1;
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
        const char info[2] = {c, '\0'};
        usage("unknown character", info);
    }
    return 1;
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

int strIntPos(char *sub, char c)
{
    char *p = strchr(sub, c);
    return p ? p - sub : -1;
}

struct ASTnode *mkastNode(enum AstType op, struct ASTnode *left, struct ASTnode *right, int intValue)
{
    struct ASTnode *node = (struct ASTnode *)malloc(sizeof(struct ASTnode));
    if (node == NULL)
    {
        printf("unable to malloc in mkastNode \n");
        exit(1);
    }
    node->op = op;
    node->left = left;
    node->right = right;
    node->intValue = intValue;
    return node;
}

struct ASTnode *mkastLeaf(enum AstType op, int intValue)
{
    return mkastNode(op, NULL, NULL, intValue);
}

struct ASTnode *primary()
{
    struct ASTnode *n;
    switch (Token.token)
    {
    case T_INTLIT:
        n = mkastLeaf(A_INTLIT, Token.intValue);
        break;
    default:
        usage("digit", (char *)&Token.intValue);
    }
    return n;
}

enum AstType getASTop(enum TokenType token)
{
    switch (token)
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
        usage("unknown token in getASTop()", (char *)&token);
    }
}

int calcPreced(enum TokenType t)
{
    int score = tokenMap[t];
    if (score == 0)
    {
        usage("enum TokenType", (char *)&score);
    }
    return score;
}

// struct ASTnode *binexpr(int prev)
// {
//     struct ASTnode *left, *right;
//     left = primary();
//     scan(&Token);
//     enum TokenType tt = Token.token;
//     if (tt == T_EOF)
//     {
//         return left;
//     }
//     while (calcPreced(tt) > prev)
//     {
//         scan(&Token);
//         right = binexpr(tokenMap[tt]);
//         left = mkastNode(getASTop(tt), left, right, 0);
//         tt = Token.token;
//         if (tt == T_EOF)
//             break;
//     }
//     return left;
// }

struct ASTnode *binexpr(int prev)
{
    struct ASTnode *left, *right;
    left = primary();
    scan(&Token);
    enum TokenType tt = Token.token;
    if (tt == T_EOF)
        return left;
    while (calcPreced(tt) > prev)
    {
        scan(&Token);
        right = binexpr(tokenMap[tt]);
        left = mkastNode(getASTop(tt), left, right, 0);
        tt = Token.token;
        if (tt == T_EOF)
            break;
    }
    return left;
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
        usage("arg length", argv[0]);
    init();
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open source code", argv[1]);
    }
    scan(&Token);
    struct ASTnode *n = binexpr(0);
    printf("%d \n", interpretAST(n));
    return 0;
}
