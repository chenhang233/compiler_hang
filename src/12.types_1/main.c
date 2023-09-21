#include "function.h"

int init()
{
    Line = 1;
    cache = 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        custom_error_chars("argc length", (char *)argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        custom_error_chars("unable open", (char *)argv[1]);
    }
    if ((Outfile = fopen("out.s", "w")) == NULL)
    {
        custom_error_int("Unable to create out.s", 0);
        exit(1);
    }
    init();
    scan(&t_instance);
    genpreamble();
    ASTnode *tree;
    while (1)
    {
        tree = function_declaration();
        genAST(tree, NOREG, 0);
        if (t_instance.token == T_EOF)
            break;
    }
    fclose(Outfile);
    return 0;
}
