#include "data.h"

static void usage(char *err_type, char const *p)
{
    printf("%s error: %s\n", err_type, p);
    exit(1);
}

static void init()
{
    Putback = '\n';
    Line = 1;
}

static void scanFile()
{
    struct Token T;
    while (scan(&T))
    {
        printf("token %s", token_map[T.token]);
        if (T.token == T_INTLIT)
        {
            printf("value %d", T.intValue);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        usage("arguments length", argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("file open", argv[1]);
        exit(1);
    }
    init();
    scanFile();
    exit(0);
}

int scan(struct Token *T)
{
    int c = skip();
    switch (c)
    {
    case EOF:
        return 0;
        break;
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
        usage("Invalid character", "c ");
        exit(1);
    }
    return 1;
}

static int skip()
{
    int c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c)
    {
        c = next();
    }
    return c;
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

static int scanInt(int c)
{
    int k, v = 0;
    while ((k = strIntPos("0123456789", c)) >= 0)
    {
        v = v * 10 + k;
        c = next();
    }
    Putback = c;
    return v;
}

static int strIntPos(char *str, char c)
{
    char *p = strchr(str, c);
    return p ? p - str : -1;
}