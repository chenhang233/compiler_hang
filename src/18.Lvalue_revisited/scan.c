#include "function.h"

int scan(Token *t)
{
    int token_type;
    if (Rejtoken != NULL)
    {
        t = Rejtoken;
        Rejtoken = NULL;
        return 1;
    }
    int c = skip();
    switch (c)
    {
    case EOF:
        t->token = T_EOF;
        return (0);
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
    case ';':
        t->token = T_SEMI;
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
    case '=':
        if ((c = next()) == '=')
        {
            t->token = T_EQ;
        }
        else
        {
            cache = c;
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
            custom_error_char("Unrecognised character", c);
        }
        break;
    case '<':
        if ((c = next()) == '=')
        {
            t->token = T_LE;
        }
        else
        {
            cache = c;
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
            cache = c;
            t->token = T_GT;
        }
        break;
    case '&':
        if ((c = next()) == '&')
        {
            t->token = T_LOGAND;
        }
        else
        {
            cache = c;
            t->token = T_AMPER;
        }
        break;
    default:
        if (isdigit(c))
        {
            t->intvalue = scanint(c);
            t->token = T_INTLIT;
        }
        else if (isalpha(c) || '_' == c)
        {
            if (token_type = scankey(c))
            {
                t->token = token_type;
                break;
            }
            t->token = T_IDENT;
            break;
        }
        custom_error_char("Unrecognised character", c);
    }
    return 1;
}

int next()
{
    int c;
    if (cache)
    {
        c = cache;
        cache = 0;
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
int scanint(int c)
{
    int k, v = 0;
    while ((k = int_pos("0123456789", c)) != -1)
    {
        v = v * 10 + k;
        c = next();
    }
    cache = c;
    return v;
}

int int_pos(char *str, int s)
{
    char *p = strchr(str, s);
    return p ? p - str : -1;
}

int scanident(char c, char *str, int strlen)
{
}

int scankey(char *s)
{
    switch (*s)
    {
    case 'c':
        if (!strcmp(s, "char"))
            return (T_CHAR);
        break;
    case 'e':
        if (!strcmp(s, "else"))
            return (T_ELSE);
        break;
    case 'f':
        if (!strcmp(s, "for"))
            return (T_FOR);
        break;
    case 'i':
        if (!strcmp(s, "if"))
            return (T_IF);
        if (!strcmp(s, "int"))
            return (T_INT);
        break;
    case 'l':
        if (!strcmp(s, "long"))
            return (T_LONG);
        break;
    case 'r':
        if (!strcmp(s, "return"))
            return (T_RETURN);
        break;
    case 'w':
        if (!strcmp(s, "while"))
            return (T_WHILE);
        break;
    case 'v':
        if (!strcmp(s, "void"))
            return (T_VOID);
        break;
    }
    return (0);
}

static Token *Rejtoken = NULL;

void reject_token(Token *t)
{
    if (!Rejtoken)
        custom_error_int("Can't reject token twice", 0);
    Rejtoken = t;
}