#include "function.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
        custom_error_chars("argc length", argv[0]);
    if (!(Infile = fopen(argv[1], "r")))
    {
        custom_error_chars("unable open", argv[1]);
    }
    Token *t;
    scan(&t);
    while (1)
    {
    }

    return 0;
}
