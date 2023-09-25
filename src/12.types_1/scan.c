#include "function.h"

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
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t')
    {
        c = next();
    }
    return c;
}

int scan(Token *t)
{
    int c = skip();
    Token_type ttype;
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
            // printf("test\n");
            t->token = T_NE;
        }
        else
        {
            custom_error_char("unkown character !=", c);
        }
        break;
    case '<':
        if ((c = next()) == '=')
        {
            t->token = T_LE;
        }
        else
        {
            t->token = T_LT;
            cache = c;
        }
        break;
    case '>':
        if ((c = next()) == '=')
        {
            t->token = T_GE;
        }
        else
        {
            t->token = T_GT;
            cache = c;
        }
        break;
    default:
        if (isdigit(c))
        {
            t->intvalue = scanint(c);
            t->token = T_INTLIT;
        }
        else if (isalpha(c) || c == '_')
        {
            scanident(c, Text, TEXTLEN);
            if (ttype = scankey(Text))
            {
                t->token = ttype;
                break;
            }
            t->token = T_IDENT;
        }
        else
        {
            custom_error_chars("Unrecognised character", "Token_type");
            custom_error_char("unkown character ?", c);
        }
    }
    // printf(" t->token=%d\n", t->token);
    return 1;
}

int scankey(char *key)
{
    switch (*key)
    {
    case 'p':
        if (!strcmp("print", key))
            return T_PRINT;
    case 'i':
        if (!strcmp("int", key))
            return T_INT;
        if (!strcmp("if", key))
            return T_IF;
    case 'e':
        if (!strcmp("else", key))
            return T_ELSE;
    case 'w':
        if (!strcmp("while", key))
            return T_WHILE;
    case 'f':
        if (!strcmp("for", key))
            return T_FOR;
    case 'v':
        if (!strcmp("void", key))
            return T_VOID;
    case 'c':
        if (!strcmp("char", key))
            return T_CHAR;
    default:
        return 0;
    }
}

int scanident(char c, char *str, int strlen)
{
    int i = 0;
    while (isalpha(c) || isdigit(c) || c == '_')
    {
        str[i++] = c;
        if (i == strlen - 1)
            custom_error_char("too long ident", c);
        c = next();
    }
    cache = c;
    Text[i] = '\0';
    return i;
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
