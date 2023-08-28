#include <errno.h>
#include "data.h"

const char *tokenStr[] = {"+", "-", "*", "/", "intlit"};

static void init()
{
    Line = 1;
    Putback = '\n';
}

static void usage(char const *f)
{
    fprintf(stderr, "usage: %s infile\n", f);
    exit(1);
}

static void scanfile()
{
    struct Token t;
    while (scan(&t))
    {
        printf("token %s", tokenStr[t.token]);
        if (t.token == T_INTLIT)
        {
            printf(",value %d", t.intValue);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        usage(argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        fprintf(stderr, "Unable to open: %s\n", argv[1]);
        exit(1);
    }
    init();
    scanfile();
    exit(0);
}
