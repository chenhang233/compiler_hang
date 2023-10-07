#include "function.h"

int scan(Token *t)
{
    int token_type;
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
        break;
    }
    return 0;
}

int next()
{
}

int skip()
{
}

int scanint(int c)
{
}

int scanident(char c, char *str, int strlen)
{
}

int scankey(char *s)
{
}

void reject_token(Token *t)
{
}