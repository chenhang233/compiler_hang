#include <data.h>
#include <errno.h>

const char *tokenStr[] = {'+', '-', '*', '/', 'intlit'};

static void usage(char *f)
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
            printf(",value ", t.intValue);
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
    scanfile();
    exit(0);
}
