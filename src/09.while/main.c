#include "function.h"

int init()
{
    Line = '\n';
    Cache = 0;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        custom_error_chars("argc length", (char *)argv[0]);
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        custom_error_chars("open Infile", (char *)argv[1]);
    }
    if (!(Outfile = fopen("out.s", "w")))
    {
        custom_error_chars("open outfile", (char *)argv[1]);
    }
    scan(&t_instance);
    genpreamble();
    AST_node *n = compound_statement();
    genAST(n, NOREG, 0);
    genpostamble();
    fclose(Outfile);
    return 0;
}
