#include "function.h"

void init()
{
    Line = 1;
    cache = 0;
    DumpAST = 0;
}

int main(int argc, char const *argv[])
{
    int i;
    init();
    for (i = 1; i < argc; i++)
    {
        if (*argv[i] != '-')
            break;
        switch (argv[i][1])
        {
        case 'T':
            DumpAST = 1;
            break;
        default:
            custom_error_chars("cannot match cmdline", (char *)argv[0]);
        }
    }
    if (!(Infile = fopen(argv[i], "r")))
        custom_error_chars("open infile", (char *)argv[i]);
    if (!(Outfile = fopen(argv[i], "r")))
        custom_error_chars("open Outfile", (char *)argv[i]);

    addglob("printint", P_CHAR, S_FUNCTION, 0);
    scan(&t_instance);
    genpreamble();

    global_declarations();

    genpostamble();
    fclose(Outfile);
    return 0;
}
