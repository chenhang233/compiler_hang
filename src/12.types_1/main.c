#include "function.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
        custom_error_chars("argc length", argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        custom_error_chars("unable open", argv[1]);
    }
    if ((Outfile = fopen("out.s", "w")) == NULL)
    {
        custom_error_int("Unable to create out.s", 0);
        exit(1);
    }
    Token *t;
    scan(&t);
    genpreamble();
    ASTnode *tree;
    while (1)
    {
        tree = function_declaration();
        genAST(tree, NOREG, 0);
        if (t->token == T_EOF)
            break;
    }
    fclose(Outfile);
    return 0;
}
