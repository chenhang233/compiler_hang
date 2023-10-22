#include "function.h"

static Token *Rejtoken = NULL;

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
        if ((c = next()) == '+')
        {
            t->token = T_INC;
        }
        else
        {
            cache = c;
            t->token = T_PLUS;
        }
        break;
    case '-':
        if ((c = next()) == '-')
        {
            t->token = T_DEC;
        }
        else
        {
            cache = c;
            t->token = T_MINUS;
        }
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
    case '[':
        t->token = T_LBRACKET;
        break;
    case ']':
        t->token = T_RBRACKET;
        break;
    case '~':
        t->token = T_INVERT;
        break;
    case '^':
        t->token = T_XOR;
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
            cache = c;
            t->token = T_LOGNOT;
        }
        break;
    case '<':
        if ((c = next()) == '=')
        {
            t->token = T_LE;
        }
        else if (c == '<')
        {
            t->token = T_LSHIFT;
        }
        else
        {
            putback(c);
            t->token = T_LT;
        }
        break;
    case '>':
        if ((c = next()) == '=')
        {
            t->token = T_GE;
        }
        else if (c == '>')
        {
            t->token = T_RSHIFT;
        }
        else
        {
            putback(c);
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
    case '|':
        if ((c = next()) == '|')
        {
            t->token = T_LOGOR;
        }
        else
        {
            putback(c);
            t->token = T_OR;
        }
        break;
    case '\'':
        t->intvalue = scan_ch();
        t->token = T_INTLIT;
        if (next() != '\'')
            custom_error_int("Expected '\\'' at end of char literal", 0);
        break;
    case '"':
        scanstr(Text);
        t->token = T_STRLIT;
        break;
    default:
        if (isdigit(c))
        {
            t->intvalue = scanint(c);
            t->token = T_INTLIT;
            break;
        }
        else if (isalpha(c) || '_' == c)
        {
            scanident(c, Text, TEXTLEN);
            if (token_type = scankey(Text))
            {
                // printf("token_type=%d\n", token_type);
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

static int scanident(char c, char *str, int strlen)
{
    int i = 0;
    while (isalpha(c) || isdigit(c) || c == '_')
    {
        str[i++] = c;
        if (i == strlen)
            custom_error_int("scanident TEXT out of range", strlen);
        c = next();
    }
    cache = c;
    str[i] = '\0';
    return i;
}

int scankey(char *s)
{
    // printf("--%s\n", s);
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

void reject_token(Token *t)
{
    if (Rejtoken)
        custom_error_int("Can't reject token twice", 0);
    Rejtoken = t;
}

static int scan_ch()
{
    int c;
    c = next();
    while (c == '\\')
    {
        switch (c = next())
        {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case '\\':
            return '\\';
        case '"':
            return '"';
        case '\'':
            return '\'';
        default:
            custom_error_int("unknown escape sequence", c);
        }
    }
    return c;
}

static int scanstr(char *buf)
{
    int c, i = 0;
    while (1)
    {
        c = scan_ch();
        if (c == '"')
        {
            buf[i] = '\0';
            return i;
        }
        buf[i++] = c;
        if (i == TEXTLEN)
            custom_error_int("String literal too long", 0);
    }
}