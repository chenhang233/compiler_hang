#include "function.h"

void usage(char *str, const char *c)
{
    custom_error_chars(str, (char *)c);
    exit(1);
};

void init()
{
    Line = 1;
    Cache_c = '\n';
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        usage("argc not 2", argv[0]);
    }
    if (!(Infile = fopen(argv[1], "r")))
    {
        usage("open Infile", argv[1]);
    }
    init();
    scan(&T_instance);
    generator_preamble();
    statements();
    generator_postamble();
    return 0;
}