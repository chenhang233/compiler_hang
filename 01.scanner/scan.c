#include <data.h>

static int strIntPos(char *str, int c)
{
    char *p;
    p = strchr(str, c);
    return p ? p - str : -1;
}

static int next()
{
    int c;
    if (Putback)
    {
        c = Putback;
        Putback = 0;
        return c;
    }
    c = fgetc(Infile);
    if (c == '\n')
        Line++;
    return c;
}

static int skip()
{
    int c;
    c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c)
    {
        c = next();
    }
    return c;
}

static int scanInt(int c)
{
    int k, v = 0;

    while ((k = strIntPos("0123456789", c)) >= 0)
    {
        v = v * 10 + k;
        c = next();
    }
    putback(c);
    return v;
}

static void putback(int c)
{
    Putback = c;
}

int scan(struct Token *t)
{
    int c;
    c = skip();
    switch (c)
    {
    case EOF:
        return 0;
    case '+':
        t->token = T_PUT;
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
            t->intValue = scanInt(c);
            t->token = T_INTLIT;
            break;
        }
        printf("Unrecognised character %c on line %d\n", c, Line);
        exit(1);
    }
    return 1;
}