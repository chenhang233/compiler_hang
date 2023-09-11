#include "function.h"

int next()
{
    int c;
    if (Cache)
    {
        c = Cache;
        Cache = 0;
        return c;
    }
    c = fgetc(Infile);
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

int scan(Token *t)
{
    int c = skip();
    Token_type type;
    switch (c)
    {
    case EOF:
        t->token = T_EOF;
        return 0;
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_STAR;
        break;
    case '/':
        t->token = T_SLASH;
        break;
    case '{':
        t->token = T_LBRACE;
        break;
    case '}':
        t->token = T_RBRACE;
        break;
    case '(':
        t->token = T_LPAREN;
        break;
    case ')':
        t->token = T_RPAREN;
        break;
    case ';':
        t->token = T_SEMI;
        break;
    case '=':
        if ((c = next()) == '=')
        {
            t->token = T_EQ;
        }
        else
        {
            Cache = c;
            t->token = T_ASSIGN;
        }
        break;
    case '!':
        if ((c = next()) == '=')
        {
            t->token = T_NE;
        }
        else
        {
            custom_error_chars("Unrecognised character", "!=");
        }
        break;
    case '<':
        if ((c = next()) == '=')
        {
            t->token = T_LE;
        }
        else
        {
            Cache = c;
            t->token = T_LT;
        }
        break;
    case '>':
        if ((c = next()) == '=')
        {
            t->token = T_GE;
        }
        else
        {
            Cache = c;
            t->token = T_GT;
        }
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->v = scanint(c);
        }
        else if (isalpha(c) || c == '_')
        {
            scanident(c, Text, TEXT_LEN);
            if (type = keyword(Text))
            {
                t->token = type;
            }
            else
            {
                t->token = T_IDENT;
            }
            break;
        }
        custom_error_chars("Unrecognised character", "Token_type");
    }
    return 1;
}

int keyword(char *key)
{
}

int scanident(int c, char *Text, int size)
{
    int i = 0;
    while (isdigit(c) || isalpha(c) || c == '_')
    {
        Text[i++] = c;
        if (i == size)
        {
            custom_error_int("too long TEXT", TEXT_LEN);
        }
        c = next();
    }
    Cache = c;
}

int int_pos(char *str, int s)
{
    char *p = strchr(str, s);
    return p ? p - str : -1;
}

int scanint(int c)
{
    int t, v = 0;
    while ((t = int_pos("0123456789", c)) != -1)
    {
        v = v * 10 + t;
        c = next();
    }
    Cache = c;
    return v;
}