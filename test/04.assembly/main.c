#include "data.h"

void usage(char *ei)
{
    printf("ERROR: %s, line: %d\n", ei, Line);
    exit(1);
}

void init()
{
    Line = 1;
    Input_cache = '\n';
}

int next()
{
    int c;
    if (Input_cache)
    {
        c = Input_cache;
        Input_cache = 0;
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
    while (c == '\n' || c == ' ' || c == '\r' || c == '\t')
    {
        c = next();
    }
    return c;
}

int str_digit_index(char *s, char c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scan_digit(int c)
{
    int t, v = 0;
    while ((t = str_digit_index("0123456789", c)) >= 0)
    {
        v = v * 10 + t;
        c = next();
    }
    Input_cache = c;
    return v;
}

int scan(Token *t)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        return 0;
        t->token = T_EOF;
    case '+':
        t->token = T_ADD;
        break;
    case '-':
        t->token = T_SUB;
        break;
    case '*':
        t->token = T_MUL;
        break;
    case '/':
        t->token = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->v = scan_digit(c);
            break;
        }
        usage("Unknown character");
    }
    return 1;
}

AST_node *mk_AST_node(AST_type op, AST_node *l, AST_node *r, int v)
{
    AST_node *node = malloc(sizeof(AST_node));
    node->l = l;
    node->r = r;
    node->op = op;
    node->v = v;
    return node;
}

AST_node *mk_AST_leaf(AST_type op, int v)
{
    return mk_AST_node(op, NULL, NULL, v);
}

AST_node *parse_INTLIT()
{
    switch (T.token)
    {
    case T_INTLIT:
        return mk_AST_leaf(A_INTLIT, T.v);
        break;
    default:
        usage("A number is needed, but something else is entered");
    }
}

int get_priority_symbol(Token_type i)
{
    int priority = priority_symbol[i];
    if (priority == 0)
    {
        printf("i=%d\n", i);
        usage("Need operator");
    }
    return priority;
}
AST_type get_AST_type(Token_type t)
{
    switch (t)
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
        usage("get_AST_type");
    }
}

AST_node *parse_AST(int prev)
{
    AST_node *l, *r;
    Token_type t;
    l = parse_INTLIT();
    scan(&T);
    t = T.token;
    if (t == T_EOF)
        return l;
    while (get_priority_symbol(t) > prev)
    {
        scan(&T);
        r = parse_AST(priority_symbol[t]);
        l = mk_AST_node(get_AST_type(t), l, r, 0);
        t = T.token;
        if (t == T_EOF)
        {
            break;
        }
    }
    return l;
}

int interpret_AST(AST_node *n)
{
    int l, r;
    if (n->l)
        interpret_AST(n->l);
    if (n->r)
        interpret_AST(n->r);
    switch (n->op)
    {
    case A_INTLIT:
        return n->v;
    case A_ADD:
        return l + r;
    case A_SUB:
        return l - r;
    case A_MUL:
        return l * r;
    case A_DIV:
        return l / r;
    default:
        usage("interpret_AST");
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc length");
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open infile");
    }
    init();
    scan(&T);
    AST_node *ast = parse_AST(0);
    printf("%d\n", interpret_AST(ast));
    return 0;
}
