#include "function.h"

int next()
{
    int c;
    if (Cache_c)
    {
        c = Cache_c;
        Cache_c = 0;
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
    while (c == '\n' || c == ' ' || c == '\r' || c == '\t' || c == '\f')
    {
        c = next();
    }
    return c;
}

int dight_pos(char *s, int c)
{
    char *p = strchr(s, c);
    return p ? p - s : -1;
}

int scan_dight(int d)
{
    int t, v = 0;
    while ((t = dight_pos("0123456789", d)) >= 0)
    {
        v = v * 10 + t;
        d = next();
    }
    Cache_c = d;
    return v;
}

int scan_identifier(int c, char *buf, int buf_size)
{
    int i = 0;
    while (isalpha(c) || isdigit(c) || c == '_')
    {
        buf[i++] = c;
        if (i == buf_size)
        {
            custom_error_char("too long identifier", c);
            exit(0);
        }
        c = next();
    }
    buf[i] = '\0';
    Cache_c = c;
    return i;
}

int scan(Token *t)
{
    int c = skip();
    Token_type type;
    switch (c)
    {
    case EOF:
        return 0;
    case '+':
        t->type = T_ADD;
        break;
    case '-':
        t->type = T_SUB;
        break;
    case '*':
        t->type = T_MUL;
        break;
    case '/':
        t->type = T_DIV;
        break;
    default:
        if (isdigit(c))
        {
            t->type = T_INTLIT;
            t->v.int_v = scan_dight(c);
        }
        else if (isalpha(c) || c == '_')
        {
            scan_identifier(c, TEXT, TEXT_LEN);
            if (type = scan_keys(TEXT))
            {
                t->type = type;
                break;
            }
            t->type = T_IDENT;
            break;
        }
        // errr
        break;
    }
}