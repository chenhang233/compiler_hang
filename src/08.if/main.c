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
    scan(&t_instance);
    return 0;
}
